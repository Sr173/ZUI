#include "Test.h"
#include <Windows.h>
using namespace ZUI;
void Test_IsRectCross()
{
	RECT rects[] = {
		{ 0, 0, 10, 10 }, { 20, 20, 30, 30 }, /* ���ཻ */
		{ 20, 20, 30, 30 }, { 0, 0, 10, 10 }, /* ���ཻ */
		{ 0, 0, 10, 10 }, { 0, 0, 10, 10 }, /* �غ� */
		{ 30, 30, 40, 40 }, { 20, 20, 60, 60 }, /* ���� */
		{ 30, 30, 70, 70 }, { 60, 40, 110, 90 }, /* ���� */
		{ 30, 30, 70, 70 }, { 30, 70, 70, 110 }, /* ƽ�� */
		{ 30, 30, 70, 70 }, { 30, 71, 70, 111 }, /* */
	};
	bool bs[] = { false, false, true, true, true, false, false };
	for (int i = 0; i < 7; ++i) {
		assert(IsRectCross(rects[2*i], rects[2*i + 1])
			== bs[i]);
	}

}
void ZFuncTest()
{
	Test_IsRectCross();
}