#include "Test.h"
using namespace ZUI;
#include <stdio.h>
#include <iostream>
typedef void(*func1)(int);
class OneEvent :
	public ZEvent<func1>
{
public:
	void Invoke(int value)
	{
		for each (auto func in m_funcs)
		{
			func(value);
		}
	}
};
class someclass
{
public:
	void dosomething(int value) {
		printf("%d\n", value * 2);
	}
};
void printsome(int value)
{
	printf("%d\n", value);
}
void ZEventTest()
{
	OneEvent event1;
	event1.subscribe(printsome);
	event1.Invoke(2);
}