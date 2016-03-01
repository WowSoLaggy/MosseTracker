#include "stdafx.h"
#include "Test4Trackers.h"


void Test4Trackers::Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH)
{
	int curFrame = pStartFrame;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb, imGray;

	int rect1X = pX;
	int rect1Y = pY;
	int rect1W = pW;
	int rect1H = pH;

	int rect2X = pX + pW / 2;
	int rect2Y = pY;
	int rect2W = pW;
	int rect2H = pH;

	int rect3X = pX;
	int rect3Y = pY + pH / 2;
	int rect3W = pW;
	int rect3H = pH;

	int rect4X = pX + pW / 2;
	int rect4Y = pY + pH / 2;
	int rect4W = pW;
	int rect4H = pH;

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
			Mosse_Init(imGray.ptr(), (int)imGray.step, rect3X, rect3Y, rect3W, rect3H, learnRate);
			Mosse_Init(imGray.ptr(), (int)imGray.step, rect4X, rect4Y, rect4W, rect4H, learnRate);
		}
		else
		{
			QueryPerformanceCounter(&accTimePrecStart);

			Mosse_OnFrame(0, imGray.ptr(), (int)imGray.step, rect1X, rect1Y, rect1W, rect1H);
			Mosse_OnFrame(1, imGray.ptr(), (int)imGray.step, rect2X, rect2Y, rect2W, rect2H);
			Mosse_OnFrame(2, imGray.ptr(), (int)imGray.step, rect3X, rect3Y, rect3W, rect3H);
			Mosse_OnFrame(3, imGray.ptr(), (int)imGray.step, rect4X, rect4Y, rect4W, rect4H);

			// Get new rects
			int newX = (rect1X + rect2X + rect3X + rect4X + (rect1W + rect2W + rect3W + rect4W) / 2) / 4;
			int newY = (rect1Y + rect2Y + rect3Y + rect4Y + (rect1H + rect2H + rect3H + rect4H) / 2) / 4;
			rect1X = newX - rect1W * 3 / 4; rect1Y = newY - rect1H * 3 / 4;
			rect2X = newX - rect2W / 4; rect2Y = newY - rect2H * 3 / 4;
			rect3X = newX - rect3W * 3 / 4; rect3Y = newY - rect3H / 4;
			rect4X = newX - rect4W / 4; rect4Y = newY - rect4H / 4;

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
		cv::rectangle(imGray, cv::Rect(rect2X, rect2Y, rect2W, rect2H), 0, 1);
		cv::rectangle(imGray, cv::Rect(rect3X, rect3Y, rect3W, rect3H), 0, 1);
		cv::rectangle(imGray, cv::Rect(rect4X, rect4Y, rect4W, rect4H), 255, 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}
