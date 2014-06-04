#include "stdafx.h"
#include "ZCommonWnd.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
namespace ZUI
{
	namespace ZCommonWnd
	{
		//CmnMsgWnd
		void CmnMsgWnd::SetText(LPCWSTR text)
		{
			if (m_lpText != NULL) {
				delete[] m_lpText;
				m_lpText = NULL;
			}
			if (text != NULL) {
				unsigned int len = wcslen(text);
				m_lpText = new WCHAR[len + 1];
				_tcscpy_s(m_lpText, (len + 1)*sizeof(WCHAR), text);
			}
			Invalidate();
		}
		LPCTSTR CmnMsgWnd::ClassName() const
		{
			return _T("ZUI_CmnMsgWnd");
		}
		LRESULT CmnMsgWnd::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			HDC hdc;
			switch (uMsg)
			{
			case WM_PAINT:
				PAINTSTRUCT ps;
				hdc = ::BeginPaint(hWnd, &ps);
				OnPaint(hdc);
				::EndPaint(hWnd, &ps);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return 0;
		}
		void CmnMsgWnd::OnPaint(HDC hdc)
		{
			if (m_lpText == NULL) return;
			GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR           gdiplusToken;
			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			Graphics graphics(hdc);
			graphics.Clear(Color(255, 255, 255, 255));
			SolidBrush brush(Color(255, 0, 0, 0));
			FontFamily fontFamily(L"Times New Roman");
			Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			
			RECT rc;
			GetClientRect(&rc);
			float x = 0.f;
			float y = static_cast<float>(rc.bottom / 2);
			PointF pt(x, y);
			graphics.DrawString(m_lpText, -1, &font, pt, &brush);

			GdiplusShutdown(gdiplusToken);
		}
		//CmnMsgWnd
	}
}