#include "stdafx.h"


#include "SampleFromFile.h"
#include "SampleOneFrame.h"
#include "Sample4Trackers.h"
#include "SampleSwarm.h"


int main()
{
	//SampleFromFile samp;
	//SampleOneFrame samp;
	//Sample4Trackers samp;
	SampleSwarm samp;

	//samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 133, 100, 64, 64);
	//samp.Run("..\\Video\\surfing\\", ".jpg", 8, 0, 23, 55, 64, 64);
	//samp.Run("..\\Video\\surfing\\", ".jpg", 8, 0, 190, 115, 64, 64);
	//samp.Run("..\\Video\\boris\\", ".jpg", 8, 493, 475, 64, 64);
	//samp.Run("..\\Video\\car\\", ".jpg", 8, 0, 1, 145, 64, 64);
	//samp.Run("..\\Video\\david\\", ".jpg", 8, 0, 165, 80, 64, 64);
	//samp.Run("..\\Video\\sphere\\", ".jpg", 8, 0, 155, 75, 64, 64);

	//samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 133, 100, 32, 32);
	//samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 117, 84, 64, 64);

	// Not used
	/*samp.Run("..\\Video\\surfing\\", ".jpg", 8, 0,
		23, 55, 64, 64,
		200, 75, 64, 64);//*/

	// Swarm
	/*samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 10, 4,
		110, 80, 64, 64);//*/
	samp.Run("..\\Video\\dog\\", ".jpg", 8, 0, 8, 3,
		140, 110, 32, 32);//*/

    return 0;
}
