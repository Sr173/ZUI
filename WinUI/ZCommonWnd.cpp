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
		BOOL CmnMsgWnd::Create(LPCWSTR lpWindowName, int nWidth, int nHeight, LPCWSTR text, HWND parent)
		{
			RECT rc;
			if (parent == NULL) {
				HWND hwnd = ::GetDesktopWindow();
				::GetWindowRect(hwnd, &rc);
			}
			else {
				::GetWindowRect(parent, &rc);
			}
			int x = (rc.right + rc.left - nWidth) / 2;
			int y = (rc.bottom + rc.top - nHeight) / 2;
			BOOL bret = ZWinBase<CmnMsgWnd>::Create(lpWindowName,
				WS_BORDER, 0L, x, y, nWidth, nHeight, parent);
			if (text != NULL && bret) {
				SetText(text);
			}
			return bret;
		}
		CmnMsgWnd& CmnMsgWnd::SetText(LPCWSTR text)
		{
			if (m_lpText != NULL) {
				m_lpText = NULL;
			}
			Invalidate();
			return *this;
		}
		CmnMsgWnd& CmnMsgWnd::SetTextColor(BYTE r, BYTE g, BYTE b, BYTE a)
		{
			m_fontColor.r = r;
			m_fontColor.g = g;
			m_fontColor.b = b;
			m_fontColor.a = a;
			Invalidate();
			return *this;
		}

		CmnMsgWnd& CmnMsgWnd::SetBackColor(BYTE r, BYTE g, BYTE b, BYTE a)
		{
			m_backColor.r = r;
			m_backColor.g = g;
			m_backColor.b = b;
			m_backColor.a = a;
			Invalidate();
			return *this;
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
			case WM_SIZE:
				Invalidate();
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

			Graphics graphics(hdc);
			Gdiplus::Color backColor(m_backColor.a,
				m_backColor.r, m_backColor.g, m_backColor.b);
			Gdiplus::Color fontColor(m_fontColor.a,
				m_fontColor.r, m_fontColor.g, m_fontColor.b);
			graphics.Clear(backColor);
			SolidBrush brush(fontColor);
			FontFamily fontFamily(L"Times New Roman");
			Font font(&fontFamily, 
				static_cast<Gdiplus::REAL>(m_fontSize),
				FontStyleRegular, UnitPixel);
			
			RECT rc;
			GetClientRect(&rc);
			float x = static_cast<float>(rc.right / 2);
			float y = static_cast<float>(rc.bottom / 2);
			PointF pt(x, y);
			StringFormat format;
			format.SetAlignment(StringAlignment::StringAlignmentCenter);
			format.SetLineAlignment(StringAlignment::StringAlignmentCenter);
			graphics.DrawString(m_lpText.c_str(), -1, &font, pt, &format, &brush);
		}
		//CmnMsgWnd
	}
}