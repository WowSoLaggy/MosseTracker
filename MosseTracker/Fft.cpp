#include "stdafx.h"
#include "Fft.h"

#include "Fft_arrays.h"


inline void mulComplex(float &re1, float &im1, float re2, float im2)
{
	float tmp = re1;
	re1 = tmp * re2 - im1 * im2;
	im1 = tmp * im2 + im1 * re2;
}


void Fft::Init(int pLogN)
{
	m_logN = pLogN;
	m_nMax = 1 << m_logN;

	// Rotation multiplier array allocation
	m_wStore_re.resize(m_nMax / 2);
	m_wStore_im.resize(m_nMax / 2);
}

void Fft::TransformDirect(std::vector<float> &pRe, std::vector<float> &pIm)
{
	float tmp_re;
	float tmp_im;

	// First interchanging

	unsigned int n1, n2;
	unsigned char *ptrN1 = (unsigned char *)&n1;
	unsigned char *ptrN2 = (unsigned char *)&n2;
	for (n1 = 1; n1 < m_nMax - 1; n1++)
	{
		ptrN2[0] = g_reverse256[ptrN1[3]];
		ptrN2[1] = g_reverse256[ptrN1[2]];
		ptrN2[2] = g_reverse256[ptrN1[1]];
		ptrN2[3] = g_reverse256[ptrN1[0]];
		n2 >>= (32 - m_logN);
		if (n1 < n2)
		{
			tmp_re = pRe[n1];
			tmp_im = pIm[n1];
			pRe[n1] = pRe[n2];
			pIm[n1] = pIm[n2];
			pRe[n2] = tmp_re;
			pIm[n2] = tmp_im;
		}
	}

	// Reset rotation multiplier array

	m_wStore_re[0] = 1.0f;
	m_wStore_im[0] = 0.0f;

	// Main loop

	unsigned int N, Nd2;
	float *ptrW2n_re, *ptrW2n_im;
	unsigned int skew, k, m, mpNd2;
	float wn_re, wn_im, w_re, w_im;
	float *ptrWArray_re, *ptrWArray_im;
	for (N = 2, Nd2 = 1, ptrW2n_re = g_w2n_re, ptrW2n_im = g_w2n_im, skew = m_nMax >> 1;
	N <= m_nMax;
		Nd2 = N, N += N, ptrW2n_re++, ptrW2n_im++, skew >>= 1)
	{
		// wn = W(1, N) = exp(-2 * pi * j / N)
		wn_re = *ptrW2n_re;
		wn_im = *ptrW2n_im;

		for (ptrWArray_re = &m_wStore_re[0], ptrWArray_im = &m_wStore_im[0], k = 0;
		k < Nd2;
			k++, ptrWArray_re += skew, ptrWArray_im += skew)
		{
			if (k & 1)
			{
				mulComplex(w_re, w_im, wn_re, wn_im);
				*ptrWArray_re = w_re;
				*ptrWArray_im = w_im;
			}
			else
			{
				w_re = *ptrWArray_re;
				w_im = *ptrWArray_im;
			}

			for (m = k; m < m_nMax; m += N)
			{
				mpNd2 = m + Nd2;

				tmp_re = w_re;
				tmp_im = w_im;
				mulComplex(tmp_re, tmp_im, pRe[mpNd2], pIm[mpNd2]);
				pRe[mpNd2] = pRe[m];
				pIm[mpNd2] = pIm[m];
				pRe[mpNd2] -= tmp_re;
				pIm[mpNd2] -= tmp_im;
				pRe[m] += tmp_re;
				pIm[m] += tmp_im;
			}
		}
	}
}

void Fft::TransformComplement(std::vector<float> &pRe, std::vector<float> &pIm)
{
	float tmp_re;
	float tmp_im;

	// First interchanging

	unsigned int n1, n2;
	unsigned char *ptrN1 = (unsigned char *)&n1;
	unsigned char *ptrN2 = (unsigned char *)&n2;
	for (n1 = 1; n1 < m_nMax - 1; n1++)
	{
		ptrN2[0] = g_reverse256[ptrN1[3]];
		ptrN2[1] = g_reverse256[ptrN1[2]];
		ptrN2[2] = g_reverse256[ptrN1[1]];
		ptrN2[3] = g_reverse256[ptrN1[0]];
		n2 >>= (32 - m_logN);
		if (n1 < n2)
		{
			tmp_re = pRe[n1];
			tmp_im = pIm[n1];
			pRe[n1] = pRe[n2];
			pIm[n1] = pIm[n2];
			pRe[n2] = tmp_re;
			pIm[n2] = tmp_im;
		}
	}

	// Reset rotation multiplier array

	m_wStore_re[0] = 1.0;
	m_wStore_im[0] = 0.0;

	// Main loop

	unsigned int N, Nd2;
	float *ptrW2n_re, *ptrW2n_im;
	unsigned int skew, k, m, mpNd2;
	float wn_re, wn_im, w_re, w_im;
	float *ptrWArray_re, *ptrWArray_im;
	for (N = 2, Nd2 = 1, ptrW2n_re = g_w2n_re, ptrW2n_im = g_w2n_im, skew = m_nMax >> 1;
	N <= m_nMax;
		Nd2 = N, N += N, ptrW2n_re++, ptrW2n_im++, skew >>= 1)
	{
		// wn = W(1, N) = exp(-2 * pi * j / N)
		wn_re = *ptrW2n_re;
		wn_im = -(*ptrW2n_im);	// Minus because it is a complement transform

		for (ptrWArray_re = &m_wStore_re[0], ptrWArray_im = &m_wStore_im[0], k = 0;
		k < Nd2;
			k++, ptrWArray_re += skew, ptrWArray_im += skew)
		{
			if (k & 1)
			{
				mulComplex(w_re, w_im, wn_re, wn_im);
				*ptrWArray_re = w_re;
				*ptrWArray_im = w_im;
			}
			else
			{
				w_re = *ptrWArray_re;
				w_im = *ptrWArray_im;
			}

			for (m = k; m < m_nMax; m += N)
			{
				mpNd2 = m + Nd2;

				tmp_re = w_re;
				tmp_im = w_im;
				mulComplex(tmp_re, tmp_im, pRe[mpNd2], pIm[mpNd2]);
				pRe[mpNd2] = pRe[m];
				pIm[mpNd2] = pIm[m];
				pRe[mpNd2] -= tmp_re;
				pIm[mpNd2] -= tmp_im;
				pRe[m] += tmp_re;
				pIm[m] += tmp_im;
			}
		}
	}

	// Because it is a complement transform
	for (n1 = 0; n1 < m_nMax; n1++)
	{
		pRe[n1] /= m_nMax;
		pIm[n1] /= m_nMax;
	}
}
