#include "stdafx.h"


#include "TestFromFile.h"
#include "TestOneFrame.h"


int main()
{
	TestFromFile test;
	//TestOneFrame test;

	//test.Run("d:\\Work\\MosseTracker\\Video\\dog\\", 8, 133, 100, 64, 64);
	//test.Run("d:\\Work\\MosseTracker\\Video\\surfing\\", 8, 23, 55, 64, 64);
	test.Run("d:\\Work\\MosseTracker\\Video\\boris\\", 8, 493, 475, 64, 64);

    return 0;
}
