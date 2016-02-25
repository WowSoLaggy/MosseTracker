#include "stdafx.h"


#include "TestFromFile.h"


int main()
{
	TestFromFile testFromFile;
	testFromFile.Run("d:\\Work\\MosseTracker\\Video\\dog\\", 8, 133, 100, 64, 64);
	//testFromFile.Run("d:\\Work\\MosseTracker\\Video\\dog\\", 8, 153, 130, 16, 16);
	//testFromFile.Run("d:\\Work\\MosseTracker\\Video\\simple\\", 1, 0, 0, 64, 64);

    return 0;
}
