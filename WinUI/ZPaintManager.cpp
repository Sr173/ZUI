#include "stdafx.h"
#include "ZPaintManager.h"
#include <assert.h>
namespace ZUI
{
	bool ZPaintManager::Setup(ZString engine)
	{
		if (engine.ToLower() == _T("gdiplus")) {
			return Setup(RMGDIPlus);
		}
		else if (engine.ToLower() == _T("direct2d")) {
			return Setup(RMDirect2D);
		}
		return false;
	}
	bool ZPaintManager::Setup(RenderMode mode) {
		m_mode = mode;
		return ResouceInit();
	}
	ZRender* ZPaintManager::CreateRender(HWND hWnd)
	{
		ZRender* ret = nullptr;
		switch (m_mode)
		{
		case RMGDIPlus:
			ret = CreateGdiRender(hWnd);
			break;
		case RMDirect2D:
			ret = CreateDirectRender(hWnd);
			break;
		}
		return ret;
	}
	bool ZPaintManager::ResouceInit()
	{
		bool ret = false;
		switch (m_mode)
		{
		case RMGDIPlus:
			ret = GdiResourceInit();
			break;
		case RMDirect2D:
			ret = DX2DResourceInit();
			break;
		}
		return ret;
	}

	void ZPaintManager::Clear()
	{
		switch (m_mode)
		{
		case RMGDIPlus:
			GdiClear();
			break;
		case RMDirect2D:
			DX2DClear();
			break;
		}
	}
	bool ZPaintManager::GdiResourceInit()
	{
		return true;
	}
	ZRender* ZPaintManager::CreateGdiRender(HWND hWnd)
	{
		return nullptr;
	}
	void ZPaintManager::GdiClear()
	{
	}
	bool ZPaintManager::DX2DResourceInit()
	{
		return true;
	}
	ZRender* ZPaintManager::CreateDirectRender(HWND hWnd)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		ZSize size;
		size.width = rc.right - rc.left;
		size.height = rc.bottom - rc.top;
		ZRender* pdxRender = CreateDirect2DRender(hWnd, size);
		return pdxRender;
	}
	void ZPaintManager::DX2DClear()
	{
	}
} //namespace ZUI