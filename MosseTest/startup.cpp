#include "stdafx.h"


#include "TestFromFile.h"
#include "TestOneFrame.h"


int main()
{
	TestFromFile test;
	//TestOneFrame test;

	test.Run("..\\Video\\dog\\", ".jpg", 8, 0, 133, 100, 64, 64);
	//test.Run("..\\Video\\surfing\\", ".jpg", 8, 0, 23, 55, 64, 64);
	//test.Run("..\\Video\\boris\\", ".jpg", 8, 493, 475, 64, 64);

    return 0;
}
