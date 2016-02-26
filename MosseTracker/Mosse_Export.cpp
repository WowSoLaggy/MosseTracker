#include "stdafx.h"
#include "Export/Mosse_Export.h"


#include "MosseTracker.h"


std::shared_ptr<MosseTracker> g_mosseTracker;


void Mosse_Init(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH, float pLearnRate)
{
	g_mosseTracker = std::make_shared<MosseTracker>();
	g_mosseTracker->Init(pScan0, pStride, pX, pY, pW, pH, pLearnRate);
}

void Mosse_OnFrame(const unsigned char *pScan0, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	g_mosseTracker->OnFrame(pScan0, pStride, pX, pY, pW, pH);
}

void Mosse_Dispose()
{
	g_mosseTracker->Dispose();
	g_mosseTracker.reset();
}


float * Mosse_GetResponse()
{
	return g_mosseTracker->GetResponse();
}
