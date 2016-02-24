// MosseTracker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MosseTracker.h"


// This is an example of an exported variable
MOSSETRACKER_API int nMosseTracker=0;

// This is an example of an exported function.
MOSSETRACKER_API int fnMosseTracker(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see MosseTracker.h for the class definition
CMosseTracker::CMosseTracker()
{
    return;
}
