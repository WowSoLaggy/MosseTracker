#pragma once

#ifndef MOSSETRACKER_H
#define MOSSETRACKER_H


#include "Fft.h"


class MosseTracker
{
public:

	void Init(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH, float pLearnRate);
	void OnFrame(const unsigned char *pScan0, int pStride, int &pX, int &pY, int &pW, int &pH);

private:

	Fft m_fft;
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
};


#endif // MOSSETRACKER_H
