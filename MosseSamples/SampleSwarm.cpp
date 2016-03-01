#include "stdafx.h"
#include "SampleSwarm.h"


void SampleSwarm::Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pRectNum, int pRectStep,
	int pLeft, int pTop, int pRectW, int pRectH)
{
	int curFrame = pStartFrame;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb, imGray;

	m_top = pTop;
	m_left = pLeft;
	m_rectNum = pRectNum;
	m_rectStepX = (float)pRectStep;
	m_rectStepY = (float)pRectStep;
	m_rectW = pRectW;
	m_rectH = pRectH;
	m_rects = new Rect[pRectNum * pRectNum];
	m_centerX = (int)(m_left + (m_rectNum - 1) * m_rectStepX / 2 + m_rectW / 2);
	m_centerY = (int)(m_top + (m_rectNum - 1) * m_rectStepY / 2 + m_rectH / 2);
	m_scaleX = 1;
	m_scaleY = 1;
	m_scale = 1;

	// Set rects to their initial positions and get orig offsets
	ArrangeRects();

	// The Mosse Tracker learning rate. Read the paper for more information
	float learnRate = 0.08f;

	// For FPS counting
	double dt = 0;
	double dtAcc = 0;
	bool firstFps = true;
	double alpha = 0.05;
	LARGE_INTEGER accTimePrecStart;
	LARGE_INTEGER accTimePrecEnd;
	LARGE_INTEGER accTimePrecFreq;
	QueryPerformanceFrequency(&accTimePrecFreq);

	while (true)
	{
		// Generate new file name
		curFrameName = std::to_string(curFrame);
		curFrameName.insert(0, pZeros - curFrameName.length(), '0');
		curFrameName = std::string(pWorkDir).append(curFrameName).append(pFileExt);

		// Read file
		imRgb = cv::imread(curFrameName);
		if (imRgb.empty()) break;					// No more files
		cv::cvtColor(imRgb, imGray, CV_RGB2GRAY);	// Convert to graysacle

		// Call tracker
		if (curFrame == pStartFrame)
		{
			for (int i = 0; i < pRectNum * pRectNum; ++i)
				Mosse_Init(imGray.ptr(), (int)imGray.step, m_rects[i].X, m_rects[i].Y, m_rects[i].W, m_rects[i].H, learnRate);
		}
		else
		{
			QueryPerformanceCounter(&accTimePrecStart);

			for (int i = 0; i < pRectNum * pRectNum; ++i)
				Mosse_OnFrame(i, imGray.ptr(), (int)imGray.step, m_rects[i].X, m_rects[i].Y, m_rects[i].W, m_rects[i].H);

			// Estimate new position
			GetOffsets();
			m_left += m_dx;
			m_top += m_dy;
			m_centerX += m_dx;
			m_centerY += m_dy;

			// Estimate new scale
			GetScale();

			// Set rects to the new position
			ArrangeRects();

			// Count FPS
			QueryPerformanceCounter(&accTimePrecEnd);
			dt = (double)(accTimePrecEnd.QuadPart - accTimePrecStart.QuadPart);
			dt /= accTimePrecFreq.QuadPart;

			if (firstFps)
			{
				firstFps = false;
				dtAcc = dt;
			}
			else
				dtAcc = dt * alpha + dtAcc * (1 - alpha);
			//std::cout << "FPS: " << (1.0 / dtAcc) << std::endl;
			std::cout << "x: " << m_scaleX << ", y: " << m_scaleY << "\n";
		}

		// Draw result
		for (int i = 0; i < pRectNum * pRectNum; ++i)
			cv::rectangle(imGray, cv::Rect(m_rects[i].X, m_rects[i].Y, m_rects[i].W, m_rects[i].H), 255, 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	delete[] m_rects;
	cv::waitKey(10000);
}

void SampleSwarm::ArrangeRects()
{
	for (int y = 0; y < m_rectNum; ++y)
	{
		for (int x = 0; x < m_rectNum; ++x)
		{
			m_rects[x + y * m_rectNum].X = (int)(m_left + m_rectStepX * x);
			m_rects[x + y * m_rectNum].Y = (int)(m_top + m_rectStepY * y);

			m_rects[x + y * m_rectNum].W = m_rectW;
			m_rects[x + y * m_rectNum].H = m_rectH;
		}
	}
}

void SampleSwarm::GetOffsets()
{
	m_dx = 0;
	m_dy = 0;

	for (int y = 0; y < m_rectNum; ++y)
	{
		for (int x = 0; x < m_rectNum; ++x)
		{
			m_dx += (int)(m_rects[x + y * m_rectNum].X - (m_left + m_rectStepX * x));
			m_dy += (int)(m_rects[x + y * m_rectNum].Y - (m_top + m_rectStepY * y));
		}
	}

	m_dx /= m_rectNum * m_rectNum;
	m_dy /= m_rectNum * m_rectNum;
}

void SampleSwarm::GetScale()
{
	m_scaleX = 0.0f;
	m_scaleY = 0.0f;

	for (int y = 0; y < m_rectNum; ++y)
	{
		for (int x = 0; x < m_rectNum; ++x)
		{
			m_scaleX += (float)(m_rects[x + y * m_rectNum].X - m_centerX) / (m_left + m_rectStepX * x - m_centerX) - 1;
			m_scaleY += (float)(m_rects[x + y * m_rectNum].Y - m_centerY) / (m_top + m_rectStepY * y - m_centerY) - 1;
		}
	}

	m_scaleX = 1 - m_scaleX / (m_rectNum * m_rectNum);
	m_scaleY = 1 - m_scaleY / (m_rectNum * m_rectNum);
	m_scale = (m_scaleX + m_scaleY) / 2;
}
