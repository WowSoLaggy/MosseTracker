#include "stdafx.h"
#include "MosseTracker.h"


void MosseTracker::Init(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH, float pLearnRate)
{
	// Initialization and memory allocation

	m_rectSize = pW * pH;
	int logN = (int)(log2(m_rectSize) + 0.5);
	m_fft.Init(logN);

	m_learnRate = pLearnRate;
	m_learnRateInv = 1 - m_learnRate;

	m_G_re.resize(m_rectSize, 0);
	m_G_im.resize(m_rectSize, 0);
	m_F_re.resize(m_rectSize, 0);
	m_F_im.resize(m_rectSize, 0);
	m_A_re.resize(m_rectSize, 0);
	m_A_im.resize(m_rectSize, 0);
	m_B_re.resize(m_rectSize, 0);
	//m_B_im.resize(m_rectSize, 0);	// Not used because will always be zero-filled
	m_H_re.resize(m_rectSize, 0);
	m_H_im.resize(m_rectSize, 0);
	m_R_re.resize(m_rectSize, 0);
	m_R_im.resize(m_rectSize, 0);

	// Generate G, transform G -> G*
	GenerateAndFourierG(pW, pH);

	// Copy F, transform F -> F*
	CopyAndFourierF(pScan0, pStride, pX, pY, pW, pH);

	// Calc H*
	CalcH_noAcc();
}

void MosseTracker::OnFrame(const unsigned char *pScan0, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	// Copy F, transform F -> F*
	CopyAndFourierF(pScan0, pStride, pX, pY, pW, pH);

	// Get response R* = F* * H*
	for (int i = 0; i < m_rectSize; ++i)
	{
		m_R_re[i] = m_F_re[i] * m_H_re[i] - m_F_im[i] * m_H_im[i];
		m_R_im[i] = m_F_re[i] * m_H_im[i] + m_F_im[i] * m_H_re[i];
	}

	// Complement transform R* -> R
	m_fft.TransformComplement(m_R_re, m_R_im);

	// Find the maximum response value

	float maxVal = m_R_re[0];
	int maxI = 0;
	for (int i = 0; i < m_rectSize; ++i)
	{
		if (m_R_re[i] > maxVal)
		{
			maxVal = m_R_re[i];
			maxI = i;
		}
	}

	// Update rect
	pX += (maxI % pW) - pW / 2 + 1;
	pY += (maxI / pW) - pH / 2 + 1;

	// Process the new image part
	CopyAndFourierF(pScan0, pStride, pX, pY, pW, pH);

	// Accumulate the new image part
	CalcH();
}
