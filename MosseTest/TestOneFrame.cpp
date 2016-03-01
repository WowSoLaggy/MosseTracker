#include "stdafx.h"
#include "TestOneFrame.h"


void TestOneFrame::Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH)
{
	int curFrame = pStartFrame;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb, imGray;

	int rectX = pX;
	int rectY = pY;
	int rectW = pW;
	int rectH = pH;

	float learnRate = 0.08f;	// The Mosse Tracker learning rate. Read the paper for more information
	int numToCall = 5000;

	// For FPS counting
	double dt = 0;
	LARGE_INTEGER accTimePrecStart;
	LARGE_INTEGER accTimePrecEnd;
	LARGE_INTEGER accTimePrecFreq;
	QueryPerformanceFrequency(&accTimePrecFreq);

	while (curFrame <= pStartFrame + 1)	// only the first (init frame) and the second frame
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
			Mosse_Init(imGray.ptr(), (int)imGray.step, rectX, rectY, rectW, rectH, learnRate);
		else
		{
			QueryPerformanceCounter(&accTimePrecStart);

			for (int i = 0; i < numToCall; i++)
				Mosse_OnFrame(0, imGray.ptr(), (int)imGray.step, rectX, rectY, rectW, rectH);

			// Count FPS
			QueryPerformanceCounter(&accTimePrecEnd);
			dt = (double)(accTimePrecEnd.QuadPart - accTimePrecStart.QuadPart);
			dt /= accTimePrecFreq.QuadPart;

			std::cout << "FPS: " << ((double)numToCall / dt) << std::endl;
		}

		// Draw result
		cv::rectangle(imGray, cv::Rect(rectX, rectY, rectW, rectH), cv::Scalar(255, 255, 255), 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}
