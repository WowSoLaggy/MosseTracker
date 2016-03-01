#pragma once

#ifndef MOSSETRACKER_H
#define MOSSETRACKER_H


// Defines the FFT algorithm. Uncomment the line with the required algorithm
//#define MOSSE_USE_FFT_NATIVE
#define MOSSE_USE_FFT_FFTW


#ifdef MOSSE_USE_FFT_NATIVE
#include "Fft.h"
#endif // MOSSE_USE_FFT_NATIVE

#ifdef MOSSE_USE_FFT_FFTW
#include "fftw3.h"
#endif // MOSSE_USE_FFT_FFTW


class MosseTracker
{
public:

	void Init(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH, float pLearnRate);
	void OnFrame(const unsigned char *pScan0, int pStride, int &pX, int &pY, int &pW, int &pH);
	void Train(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH);
	void Dispose();

	float * GetResponse();

private:

#ifdef MOSSE_USE_FFT_NATIVE
	Fft m_fft;
#endif // MOSSE_USE_FFT_NATIVE

#ifdef MOSSE_USE_FFT_FFTW
	fftwf_plan m_fftwPlanDirect;
	fftwf_plan m_fftwPlanComplement;
	fftwf_complex *m_fftwArray;
#endif // MOSSE_USE_FFT_FFTW

	int m_rectSize;
	float m_learnRate;
	float m_learnRateInv;

	std::vector<float> m_G_re;
	std::vector<float> m_G_im;

	std::vector<float> m_F_re;
	std::vector<float> m_F_im;

	std::vector<float> m_A_re;
	std::vector<float> m_A_im;

	std::vector<float> m_B_re;
	//std::vector<float> m_B_im;	// Not used because will always be zero-filled

	std::vector<float> m_H_re;
	std::vector<float> m_H_im;

	std::vector<float> m_R_re;		// Response
	std::vector<float> m_R_im;

	void GenerateAndFourierG(int pW, int pH);
	void CopyAndFourierF(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH);
	void CalcH_noAcc();
	void CalcH();

	void FourierDirect(std::vector<float> &pRe, std::vector<float> &pIm);
	void FourierComplement(std::vector<float> &pRe, std::vector<float> &pIm);
};


#endif // MOSSETRACKER_H
