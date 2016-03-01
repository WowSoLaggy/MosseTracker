#include "stdafx.h"
#include "MosseTracker.h"


void MosseTracker::GenerateAndFourierG(int pW, int pH)
{
	// Generate response kernel G

	/*const unsigned int kernelSize = 5;
	float tmp[kernelSize] = { 1.0f, 4.0f, 6.0f, 4.0f, 1.0f };*/
	/*const unsigned int kernelSize = 7;
	float tmp[kernelSize] = { 1.0f, 6.0f, 15.0f, 20.0f, 15.0f, 6.0f, 1.0f };*/
	const unsigned int kernelSize = 9;
	float tmp[kernelSize] = { 1.0f, 8.0f, 28.0f, 56.0f, 70.0f, 56.0f, 28.0f, 8.0f, 1.0f };
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
	FourierDirect(m_G_re, m_G_im);
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
	FourierDirect(m_F_re, m_F_im);
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
	for (int i = 0; i < m_rectSize; ++i)
	{
		// A = (G* * F*) * learnRate + A * (1 - learnRate)
		m_A_re[i] = (m_G_re[i] * m_F_re[i] + m_G_im[i] * m_F_im[i]) * m_learnRate + m_A_re[i] * m_learnRateInv;
		m_A_im[i] = (-m_G_re[i] * m_F_im[i] + m_G_im[i] * m_F_re[i]) * m_learnRate + m_A_im[i] * m_learnRateInv;

		// B = (F * F*) * learnRate + B * (1 - learnRate)
		m_B_re[i] = (m_F_re[i] * m_F_re[i] + m_F_im[i] * m_F_im[i]) * m_learnRate + m_B_re[i] * m_learnRateInv;

		// H = A / B
		m_H_re[i] = m_A_re[i] / m_B_re[i];
		m_H_im[i] = m_A_im[i] / m_B_re[i];
	}
}

void MosseTracker::FourierDirect(std::vector<float> &pRe, std::vector<float> &pIm)
{
#ifdef MOSSE_USE_FFT_NATIVE
	m_fft.TransformDirect(pRe, pIm);
#endif // MOSSE_USE_FFT_NATIVE

#ifdef MOSSE_USE_FFT_FFTW
	for (int i = 0; i < m_rectSize; ++i)
	{
		m_fftwArray[i][0] = pRe[i];
		m_fftwArray[i][1] = pIm[i];
	}
	fftwf_execute(m_fftwPlanDirect);
	for (int i = 0; i < m_rectSize; ++i)
	{
		pRe[i] = m_fftwArray[i][0];
		pIm[i] = m_fftwArray[i][1];
	}
#endif // MOSSE_USE_FFT_FFTW
}

void MosseTracker::FourierComplement(std::vector<float> &pRe, std::vector<float> &pIm)
{
#ifdef MOSSE_USE_FFT_NATIVE
	m_fft.TransformComplement(pRe, pIm);
#endif // MOSSE_USE_FFT_NATIVE

#ifdef MOSSE_USE_FFT_FFTW
	for (int i = 0; i < m_rectSize; ++i)
	{
		m_fftwArray[i][0] = pRe[i];
		m_fftwArray[i][1] = pIm[i];
	}
	fftwf_execute(m_fftwPlanComplement);
	for (int i = 0; i < m_rectSize; ++i)
	{
		pRe[i] = m_fftwArray[i][0];
		pIm[i] = m_fftwArray[i][1];
	}
#endif // MOSSE_USE_FFT_FFTW
}
