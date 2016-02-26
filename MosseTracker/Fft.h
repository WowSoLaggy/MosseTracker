// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:		FFT
// Authors:		Miroslav Voinarovsky, Anton Egorov
// Description:	Fast Fourier Transformtaion is based on the
//				freeware algorithm written by Miroslav Voinarovsky
//				with insignificant modifications
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

#ifndef FFT_H
#define FFT_H


class Fft
{
public:

	void Init(int pLogN);
	void TransformDirect(std::vector<float> &pRe, std::vector<float> &pIm);
	void TransformComplement(std::vector<float> &pRe, std::vector<float> &pIm);

private:

	int m_logN;
	unsigned int m_nMax;
	std::vector<float> m_wStore_re;
	std::vector<float> m_wStore_im;
};


#endif // FFT_H
