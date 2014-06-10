#ifndef ZUI_ZPAINTMANAGER_HEADER
#define ZUI_ZPAINTMANAGER_HEADER
#include "ZBase.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
namespace ZUI
{
	class ZRender :
		public NoCopyable
	{
	public:
		enum TextAlignMode
		{
			TAMNormal = 0,
			TAMNear = 1,
			TAMMiddle = 2,
			TAMFar = 3,
		};
		virtual ~ZRender()
		{}
	public:
		virtual bool PaintText(RECT rc, ZStringW text,
			int fontSize, ZColor fontColor, 
			TextAlignMode alignModeX, TextAlignMode alignModeY) = 0;
		virtual bool FillRectangle(RECT rc, ZColor color) = 0;
		virtual bool DrawRectangle(RECT rc, ZColor color) = 0;
		virtual bool DrawLine(ZColor color, POINT pt1, POINT pt2) = 0;
		virtual bool Clear(ZColor color) = 0;
		virtual void Release() = 0;
	};
	class ZPaintManager :
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
			m_mode(RMNoRender),
			m_render(nullptr)
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
			ResouceInit();
			m_render = CreateRender();
			if (m_render != nullptr) {
				return true;
			}
			else {
				return false;
			}
		}
		ZRender* GetRender()
		{
			return m_render;
		}
		void Invalidate()
		{
			if (m_render != nullptr) {
				m_render->Release();
			}
			m_render = CreateRender();
		}
		void SetHWND(HWND hWnd) {
			m_hWnd = hWnd;
			Invalidate();
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
		bool ResouceInit();
		ZRender* CreateRender();
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
		HWND							m_hWnd;
		RenderMode						m_mode;
		ZRender*						m_render;
		//gdi+ resource
		Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
		ULONG_PTR						gdiplusToken;
	};
	inline Gdiplus::Color ZColor2GdiColor(const ZColor& color)
	{
		return Gdiplus::Color(color.a, color.r, color.g, color.b);
	}
}
#endif //ZUI_ZPAINTMANAGER_HEADER