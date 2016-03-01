#include "stdafx.h"
#include "Export/Mosse_Export.h"


#include "MosseTracker.h"


std::vector<MosseTracker *> g_mosseTrackers;


int Mosse_Init(const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH, float pLearnRate)
{
	// Look for a free cell
	int newId;
	for (newId = 0; newId < (int)g_mosseTrackers.size(); ++newId)
	{
		if (g_mosseTrackers[newId] == nullptr)
			break;
	}

	if (newId == g_mosseTrackers.size())
		g_mosseTrackers.push_back(new MosseTracker());	// Create new
	else
		g_mosseTrackers[newId] = new MosseTracker();	// Create in the empty cell

	g_mosseTrackers[newId]->Init(pScan0, pStride, pX, pY, pW, pH, pLearnRate);

	return newId;
}

void Mosse_OnFrame(int pTrackerId, const unsigned char *pScan0, int pStride, int &pX, int &pY, int &pW, int &pH)
{
	g_mosseTrackers[pTrackerId]->OnFrame(pScan0, pStride, pX, pY, pW, pH);
}

void Mosse_Dispose(int pTrackerId)
{
	g_mosseTrackers[pTrackerId]->Dispose();
	delete g_mosseTrackers[pTrackerId];
	g_mosseTrackers[pTrackerId] = nullptr;
}

void Mosse_Train(int pTrackerId, const unsigned char *pScan0, int pStride, int pX, int pY, int pW, int pH)
{
}


float * Mosse_GetResponse(int pTrackerId)
{
	return g_mosseTrackers[pTrackerId]->GetResponse();
}
