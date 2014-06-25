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
	ZStringW strF = strE.SubString(0, 4);
	assert(strF == L"hihi");
	assert(strE.Find(L"hihi") == 0);
	assert(strE.Find(L"С��") == 8);
	assert(strE.Find(L"asdf") == -1);
	strF + L"dafas"= L"adf";
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
	ZStringA strF = strE.SubString(0, 4);
	assert(strF == "hihi");
	assert(strE.Find("hihi") == 0);
	assert(strE.Find("С��") == 11);
	assert(strE.Find("asdf") == -1);
}
template <typename T>
void UseP(ZAutoReleasePtr<T> p) {
	ZAutoReleasePtr<T> pm = p;
}
void AutoReleasePtrTest()
{
	ZAutoReleasePtr<ZPaintManager> pm = new ZPaintManager();
	ZAutoReleasePtr<ZPaintManager> pm1 = pm;
	pm1->Setup(_T("direct2d"));
	ZAutoReleasePtr<ZPaintManager> pmb = new ZPaintManager();
	pmb->Setup(_T("gdiplus"));
	pm1 = pm1;
	pm1 = nullptr;
	UseP(pm1);
	pm1 = pmb;
	pm.Swap(pmb);
	assert(pm1 == pm);
	assert(pm1 != pmb);
	pm1.Release();
}
void ZTypeTest()
{
	WStringTest();
	StringTest();
	AutoReleasePtrTest();
}