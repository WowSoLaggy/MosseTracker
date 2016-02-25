#pragma once

#ifndef MOSSETRACKER_H
#define MOSSETRACKER_H


#include "Fft.h"


class MosseTracker
{
public:

	void Init(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH);
	void OnFrame(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH);
	void Dispose();

private:

	Fft m_fft;
	int m_rectSize;

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

	std::vector<float> m_gray;		// Temp array to store grayscale image

	void GetGrayscale(unsigned char *pImageR, unsigned char *pImageG, unsigned char *pImageB, int pStride, int &pX, int &pY, int &pW, int &pH);
	void GenerateAndFourierG(int &pX, int &pY, int &pW, int &pH);
	void CopyAndFourierF();
	void CalcH_noAcc();
	void CalcH();
};


#endif // MOSSETRACKER_H
