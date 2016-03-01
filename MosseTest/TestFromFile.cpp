#include "stdafx.h"
#include "TestFromFile.h"


void TestFromFile::Run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH)
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
			Mosse_Init(imGray.ptr(), (int)imGray.step, rectX, rectY, rectW, rectH, learnRate);
		else
		{
			QueryPerformanceCounter(&accTimePrecStart);

			Mosse_OnFrame(0, imGray.ptr(), (int)imGray.step, rectX, rectY, rectW, rectH);

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
		cv::rectangle(imGray, cv::Rect(rectX, rectY, rectW, rectH), cv::Scalar(255, 255, 255), 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}
