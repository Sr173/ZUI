#include "stdafx.h"
#include "ZRender.h"
#include "ZDirect2DRender.h"
namespace ZUI
{
	ZRender* CreateDirect2DRender(HWND hWnd, ZSize size)
	{
		ZDirect2DRender* render = new ZDirect2DRender(hWnd, size);
		return render;
	}
}