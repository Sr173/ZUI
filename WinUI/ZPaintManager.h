#ifndef ZUI_ZPAINTMANAGER_HEADER
#define ZUI_ZPAINTMANAGER_HEADER
#include "ZBase.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
namespace ZUI
{
	class ZGdiplusManager :
		public NoCopyable
	{
	public:
		ZGdiplusManager()
		{
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}
		~ZGdiplusManager()
		{
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}
		void Release()
		{
			delete this;
		}
	private:
		Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
		ULONG_PTR						gdiplusToken;
	};
	inline Gdiplus::Color ZColor2GdiColor(const ZColor& color)
	{
		return Gdiplus::Color(color.a, color.r, color.g, color.b);
	}
}
#endif //ZUI_ZPAINTMANAGER_HEADER