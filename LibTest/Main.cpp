#include "Test.h"

int main()
{
	int x = 0;
	char y = [x](){; return "343";}()[x];
	ZEventTest();
	ZFuncTest();
	ZTypeTest();
	return 0;
}