#pragma once

#ifndef TESTFROMFILE_H
#define TESTFROMFILE_H


#include "Mosse_Export.h"


class SampleFromFile
{
public:

	void Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH);
};


#endif // TESTFROMFILE_H
