#include "stdafx.h"
#include "MosseTracker.h"


float * MosseTracker::GetResponse()
{
	return &m_R_re[0];
}
