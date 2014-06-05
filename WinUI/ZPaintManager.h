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
	public:
		static int MessageLoop()
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return (int)msg.wParam;
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