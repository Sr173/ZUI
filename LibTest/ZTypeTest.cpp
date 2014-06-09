#include "Test.h"
using namespace ZUI;
void WStringTest()
{
	ZStringW strA;
	ZStringW strB(L"hihi,���ѽ");
	assert(strA != strB);
	ZStringW strC(strB);
	assert(strC == strB);
	ZStringW strD(strA);
	assert(strD == strA);
	strD.Append(L"hihi,���ѽ");
	assert(strD == strB);
	ZStringW strE = L"hihi,���ѽС��";
	strD += L"С��";
	assert(strD == strE);
}
void StringTest()
{
	ZStringA strA;
	ZStringA strB("hihi,���ѽ");
	assert(strA != strB);
	ZStringA strC(strB);
	assert(strC == strB);
	ZStringA strD(strA);
	assert(strD == strA);
	strD.Append("hihi,���ѽ");
	assert(strD == strB);
	ZStringA strE = "hihi,���ѽС��";
	strD += "С��";
	assert(strD == strE);
}
void ZTypeTest()
{
	WStringTest();
	StringTest();
}