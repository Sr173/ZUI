#include "Test.h"
using namespace ZUI;
void WStringTest()
{
	ZStringW strA;
	ZStringW strB(L"hihi,你好呀");
	assert(strA != strB);
	ZStringW strC(strB);
	assert(strC == strB);
	ZStringW strD(strA);
	assert(strD == strA);
	strD.Append(L"hihi,你好呀");
	assert(strD == strB);
	ZStringW strE = L"hihi,你好呀小明";
	strD += L"小明";
	assert(strD == strE);
}
void StringTest()
{
	ZStringA strA;
	ZStringA strB("hihi,你好呀");
	assert(strA != strB);
	ZStringA strC(strB);
	assert(strC == strB);
	ZStringA strD(strA);
	assert(strD == strA);
	strD.Append("hihi,你好呀");
	assert(strD == strB);
	ZStringA strE = "hihi,你好呀小明";
	strD += "小明";
	assert(strD == strE);
}
void ZTypeTest()
{
	WStringTest();
	StringTest();
}