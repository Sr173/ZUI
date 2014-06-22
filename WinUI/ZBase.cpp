#include "stdafx.h"
#include <math.h>
#include "ZBase.h"
namespace ZUI
{
	bool IsRectCross(const ZRect& rc1, const ZRect& rc2)
	{
		LONG a_cx, a_cy;
		LONG b_cx, b_cy;
		a_cx = (rc1.left + rc1.right) / 2;
		a_cy = (rc1.bottom + rc1.top) / 2;
		b_cx = (rc2.left + rc2.right) / 2;
		b_cy = (rc2.bottom + rc2.top) / 2;
		LONG w = (rc1.right - rc1.left +
			rc2.right - rc2.left) / 2;
		LONG h = (rc1.bottom - rc1.top +
			rc2.bottom - rc2.top) / 2;

		return ((abs(a_cx - b_cx) < w) &&
			(abs(a_cy - b_cy) < h));
	}
}
