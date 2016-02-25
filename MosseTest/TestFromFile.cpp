#include "stdafx.h"
#include "TestFromFile.h"


void TestFromFile::Run(std::string pWorkDir, int pZeros, int pX, int pY, int pW, int pH)
{
	int curFrame = 0;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb, imGray;

	// For FPS counting
	double dt = 0;
	double dtAcc = 0;
	bool firstFps = true;
	double alpha = 0.05;
	/*LARGE_INTEGER accTimePrecStart;
	LARGE_INTEGER accTimePrecEnd;*/
	LARGE_INTEGER accTimePrecFreq;
	QueryPerformanceFrequency(&accTimePrecFreq);

	while (true)
	{
		// Generate new file name
		curFrameName = std::to_string(curFrame);
		curFrameName.insert(0, pZeros - curFrameName.length(), '0');
		curFrameName = std::string(pWorkDir).append(curFrameName).append(".jpg");

		// Read file
		imRgb = cv::imread(curFrameName);
		if (imRgb.empty()) break;					// No more files
		cv::cvtColor(imRgb, imGray, CV_RGB2GRAY);	// Convert to graysacle

		// Call tracker
		//imageInfo.Scan0 = imGray.ptr();
		//imageInfo.Stride = (int)imGray.step;
		//if (curFrame == 0)
		//	mosse.Init(imageInfo, rect);
		//else
		//{
		//	QueryPerformanceCounter(&accTimePrecStart);

		//	mosse.OnFrame(imageInfo, rect);

		//	// Count FPS
		//	QueryPerformanceCounter(&accTimePrecEnd);
		//	dt = (double)(accTimePrecEnd.QuadPart - accTimePrecStart.QuadPart);
		//	dt /= accTimePrecFreq.QuadPart;

		//	if (firstFps)
		//	{
		//		firstFps = false;
		//		dtAcc = dt;
		//	}
		//	else
		//		dtAcc = dt * alpha + dtAcc * (1 - alpha);
		//	std::cout << "FPS: " << (1.0 / dtAcc) << std::endl;
		//}

		//// Draw result
		//cv::rectangle(imGray, cv::Rect(rect.X, rect.Y, rect.W, rect.H), cv::Scalar(255, 255, 255), 1);

		// Show output
		cv::imshow("output", imGray);
		cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}
