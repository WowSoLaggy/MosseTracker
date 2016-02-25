#include "stdafx.h"
#include "MosseTracker.h"


void MosseTracker::GenerateAndFourierG(int pW, int pH)
{
	// Generate response kernel G

	const unsigned int kernelSize = 5;
	float tmp[kernelSize] = { 1.0, 4.0, 6.0, 4.0, 1.0 };
	unsigned int xStart = (pW - kernelSize) / 2;
	unsigned int yStart = (pH - kernelSize) / 2;
	float sum = 0;
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

void MosseTracker::CopyAndFourierF(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH)
{
	// Copy the input image F

	for (int y = 0; y < pH; ++y)
	{
		for (int x = 0; x < pW; ++x)
			m_F_re[x + y * pW] = *(pScan0 + (x + pX) + (y + pY) * pStride);
	}

	memset(&m_F_im[0], 0, m_rectSize * sizeof(float));			// Fill it's imaginary part with 0

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
	const float alpha = 0.08f;
	const float alphaPrev = 1 - alpha;

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
