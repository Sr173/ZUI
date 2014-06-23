#include "Test.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
int main()
{
	int x = 0;
	char y = [x](){; return "343";}()[x];
	ZEventTest();
	ZFuncTest();
	ZTypeTest();
	_CrtDumpMemoryLeaks();

	return 0;
}