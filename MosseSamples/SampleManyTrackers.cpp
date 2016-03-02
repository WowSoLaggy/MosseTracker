#include "stdafx.h"
#include "SampleManyTrackers.h"


void SampleManyTrackers::Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame,
	int p1X, int p1Y, int p1W, int p1H,
	int p2X, int p2Y, int p2W, int p2H)
{
	int curFrame = pStartFrame;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb, imGray;

	int rect1X = p1X;
	int rect1Y = p1Y;
	int rect1W = p1W;
	int rect1H = p1H;

	int rect2X = p2X;
	int rect2Y = p2Y;
	int rect2W = p2W;
	int rect2H = p2H;

	float learnRate = 0.08f;	// The Mosse Tracker learning rate. Read the paper for more information

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
			Mosse_Init(imGray.ptr(), (int)imGray.step, rect1X, rect1Y, rect1W, rect1H, learnRate);
			Mosse_Init(imGray.ptr(), (int)imGray.step, rect2X, rect2Y, rect2W, rect2H, learnRate);
		}
		else
		{
			QueryPerformanceCounter(&accTimePrecStart);

			Mosse_OnFrame(0, imGray.ptr(), (int)imGray.step, rect1X, rect1Y, rect1W, rect1H);
			Mosse_OnFrame(1, imGray.ptr(), (int)imGray.step, rect2X, rect2Y, rect2W, rect2H);

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
			std::cout << "FPS: " << (1.0 / dtAcc) << std::endl;
		}

		// Draw result
		cv::rectangle(imGray, cv::Rect(rect1X, rect1Y, rect1W, rect1H), 255, 1);
		cv::rectangle(imGray, cv::Rect(rect2X, rect2Y, rect2W, rect2H), 255, 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}
