// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:	MosseTracker
// Author:	Anton Egorov
// Warning:	Exported functions are NOT thread-safe, therefore
//			the control program must ensure thread safety.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#pragma once

#ifndef MOSSEEXPORTS_H
#define MOSSEEXPORTS_H


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Export defines
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#ifdef MOSSETRACKER_EXPORTS
#define MOSSE_API extern "C" __declspec(dllexport)
#else
#define MOSSE_API extern "C" __declspec(dllimport)
#endif


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Exported functions
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// Initializes the Mosse Tracker by the given image
MOSSE_API void Mosse_Init(
	const unsigned char *pScan0,	// [in] Pointer to the first element of the image
	int pStride,					// [in] Image stride
	int pX, int pY, int pW, int pH,	// [in] Initial rect
	float pLearnRate = 0.08f		// [in] The Mosse Tracker learning rate. Read the paper for more information
	);


// Processes the given image and moves the given rectangle to the new position
MOSSE_API void Mosse_OnFrame(
	const unsigned char *pScan0,		// [in]  Pointer to the first element of the image
	int pStride,						// [in]  Image stride
	int &pX, int &pY, int &pW, int &pH	// [out] Rectangle of interest to track object in, returns the new object position found by the Mosse Tracker
	);


// Disposes the Mosse Tracker and releases all the memory associated with it
MOSSE_API void Mosse_Dispose();


#endif // MOSSEEXPORTS_H
