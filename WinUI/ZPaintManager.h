#ifndef ZUI_ZPAINTMANAGER_HEADER
#define ZUI_ZPAINTMANAGER_HEADER
#include "ZBase.h"
#include <Windows.h>
#include "ZRender.h"
namespace ZUI
{
	class ZPaintManager :
		public ZObject,
		public NoCopyable
	{
	public:
		enum RenderMode
		{
			RMNoRender = 0,
			RMGDIPlus = 1,
			RMDirect2D = 2,
		};

	public:
		ZPaintManager() :
			m_mode(RMNoRender)
		{
		}
		~ZPaintManager()
		{
			Clear();
			
		}
		void Release()
		{
			delete this;
		}
	public:
		bool Setup(ZString engine);
		bool Setup(RenderMode mode) {
			m_mode = mode;
			return ResouceInit();
		}
		ZRender* CreateRender(HWND hWnd);
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
		bool ResouceInit();
		void Clear();
	private:
		//Gdi+ method
		bool GdiResourceInit();
		ZRender* CreateGdiRender(HWND hWnd);
		void GdiClear();
		//Direct2D method
		bool DX2DResourceInit();
		ZRender* CreateDirectRender(HWND hWnd);
		void DX2DClear();
	private:
		RenderMode						m_mode;
		//gdi+ resource
		Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
		ULONG_PTR						gdiplusToken;
		//direct2d resource
		CComPtr<ID2D1Factory>			m_pD2DFactory;
		CComPtr<IDWriteFactory>			m_pDWriteFactory;
	};
	inline Gdiplus::Color ZColor2GdiColor(const ZColor& color)
	{
		return Gdiplus::Color(color.a, color.r, color.g, color.b);
	}
}
#endif //ZUI_ZPAINTMANAGER_HEADER