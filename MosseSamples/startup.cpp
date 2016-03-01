#include "stdafx.h"


#include "SampleFromFile.h"
#include "SampleOneFrame.h"
#include "Sample4Trackers.h"


int main()
{
	SampleFromFile samp;
	//SampleOneFrame samp;
	//Sample4Trackers samp;

	samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 133, 100, 64, 64);
	//samp.Run("..\\Video\\surfing\\", ".jpg", 8, 0, 23, 55, 64, 64);
	//samp.Run("..\\Video\\boris\\", ".jpg", 8, 493, 475, 64, 64);

	//samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 133, 100, 32, 32);
	//samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 117, 84, 64, 64);

    return 0;
}
