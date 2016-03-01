#pragma once

#ifndef SAMPLESWARM_H
#define SAMPLESWARM_H


#include "Mosse_Export.h"


struct Rect
{
	int X;
	int Y;
	int W;
	int H;
};


class SampleSwarm
{
public:

	void Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pRectNum, int pRectStep,
		int pLeft, int pTop, int pRectW, int pRectH);

private:

	int m_left;
	int m_top;
	int m_centerX;
	int m_centerY;
	int m_rectNum;
	float m_rectStepX;
	float m_rectStepY;
	int m_rectW;
	int m_rectH;

	int m_dx;
	int m_dy;
	float m_scaleX;
	float m_scaleY;
	float m_scale;

	Rect *m_rects;

	void ArrangeRects();
	void GetOffsets();
	void GetScale();
};


#endif // SAMPLESWARM_H
