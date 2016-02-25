#include "stdafx.h"
#include "MosseTracker.h"


void MosseTracker::Init(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	// Initialization and memory allocation

	m_rectSize = pW * pH;
	int logN = (int)(log2(m_rectSize) + 0.5);
	m_fft.Init(logN);

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
	m_gray.resize(m_rectSize, 0);

	// Get grayscale image
	GetGrayscale(pImageR, pImageG, pImageB, pStride, pX, pY, pW, pH);

	// Generate G, transform G -> G*
	GenerateAndFourierG(pX, pY, pW, pH);

	// Copy F, transform F -> F*
	CopyAndFourierF();

	// Calc H*
	CalcH_noAcc();
}

void MosseTracker::OnFrame(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	// Get grayscale image
	GetGrayscale(pImageR, pImageG, pImageB, pStride, pX, pY, pW, pH);

	// Copy F, transform F -> F*
	CopyAndFourierF();

	// Get response R* = F* * H*
	for (int i = 0; i < m_rectSize; ++i)
	{
		m_R_re[i] = m_F_re[i] * m_H_re[i] - m_F_im[i] * m_H_im[i];
		m_R_im[i] = m_F_re[i] * m_H_im[i] + m_F_im[i] * m_H_re[i];
	}

	// Complement transform R* -> R
	m_fft.TransformComplement(m_R_re, m_R_im);

	// Find the maximum response value

	double maxVal = m_R_re[0];
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
	CopyAndFourierF();

	// Accumulate the new image part
	CalcH();
}

void MosseTracker::Dispose()
{
	// TODO:
}

void MosseTracker::GetGrayscale(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	// Get grayscale image

	double tmp;
	for (int y = 0; y < pH; ++y)
	{
		for (int x = 0; x < pW; ++x)
		{
			tmp = (*(pImageR + (x + pX) + (y + pY) * pStride) + *(pImageG + (x + pX) + (y + pY) * pStride) + *(pImageB + (x + pX) + (y + pY) * pStride));
			m_gray[x + y * pW] = tmp / 3;
		}
	}
}

void MosseTracker::GenerateAndFourierG(int &pX, int &pY, int &pW, int &pH)
{
	// Generate response kernel G

	const unsigned int kernelSize = 5;
	double tmp[kernelSize] = { 1.0, 4.0, 6.0, 4.0, 1.0 };
	unsigned int xStart = (pW - kernelSize) / 2;
	unsigned int yStart = (pH - kernelSize) / 2;
	double sum = 0;
	for (unsigned int y = 0; y < kernelSize; ++y)
	{
		for (unsigned int x = 0; x < kernelSize; ++x)
		{
			m_G_re[(xStart + x) + (yStart + y) * pW] = tmp[x] * tmp[y];
			sum += m_G_re[(xStart + x) + (yStart + y) * pW];
		}
	}
	for (unsigned int y = 0; y < kernelSize; ++y)
	{
		for (unsigned int x = 0; x < kernelSize; ++x)
			m_G_re[(xStart + x) + (yStart + y) * pW] /= sum;
	}

	// G -> G*

	m_fft.TransformDirect(m_G_re, m_G_im);
}

void MosseTracker::CopyAndFourierF()
{
	// Copy the input image F

	memcpy(&m_F_re[0], &m_gray[0], m_rectSize * sizeof(double));
	memset(&m_F_im[0], 0, m_rectSize * sizeof(double)); // Fill it's imaginary part with 0

	// F -> F*

	m_fft.TransformDirect(m_F_re, m_F_im);
}

void MosseTracker::CalcH_noAcc()
{
	for (int i = 0; i < m_rectSize; ++i)
	{
		// A = G* * F*
		m_A_re[i] = (m_G_re[i] * m_F_re[i] + m_G_im[i] * m_F_im[i]);
		m_A_im[i] = (-m_G_re[i] * m_F_im[i] + m_G_im[i] * m_F_re[i]);

		// B = F * F*
		m_B_re[i] = (m_F_re[i] * m_F_re[i] + m_F_im[i] * m_F_im[i]);

		// H = A / B
		m_H_re[i] = m_A_re[i] / m_B_re[i];
		m_H_im[i] = m_A_im[i] / m_B_re[i];
	}
}

void MosseTracker::CalcH()
{
	const double alpha = 0.125;
	const double alphaPrev = 1 - alpha;

	for (int i = 0; i < m_rectSize; ++i)
	{
		// A = (G* * F*) * alpha + A * (1 - alpha)
		m_A_re[i] = (m_G_re[i] * m_F_re[i] + m_G_im[i] * m_F_im[i]) * alpha + m_A_re[i] * alphaPrev;
		m_A_im[i] = (-m_G_re[i] * m_F_im[i] + m_G_im[i] * m_F_re[i]) * alpha + m_A_im[i] * alphaPrev;

		// B = (F * F*) * alpha + B * (1 - alpha)
		m_B_re[i] = (m_F_re[i] * m_F_re[i] + m_F_im[i] * m_F_im[i]) * alpha + m_B_re[i] * alphaPrev;

		// H = A / B
		m_H_re[i] = m_A_re[i] / m_B_re[i];
		m_H_im[i] = m_A_im[i] / m_B_re[i];
	}
}
