#pragma once

#ifndef SAMPLEMANYTRACKERS_H
#define SAMPLEMANYTRACKERS_H


#include "Mosse_Export.h"


class SampleManyTrackers
{
public:

	void Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame,
		int p1X, int p1Y, int p1W, int p1H,
		int p2X, int p2Y, int p2W, int p2H);
};


#endif // SAMPLEMANYTRACKERS_H
