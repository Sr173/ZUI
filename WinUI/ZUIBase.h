#ifndef ZUI_ZUIBASE_HEADER
#define ZUI_ZUIBASE_HEADER
#include "ZBase.h"
#include <Windows.h>
#include <iostream>
#include <assert.h>
namespace ZUI
{
	template <class WndCls>
	class ZWinBase :
		public ZObject
	{
	public:
		ZWinBase() :
			m_hWnd(NULL)
		{}
		virtual ~ZWinBase()
		{}
	public:
		BOOL Create(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
			)
		{
			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.lpfnWndProc = WndCls::WndProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = ClassName();
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor(NULL, GetDefaultCursor());
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			ATOM ret = ::RegisterClassEx(&wc);
			m_hWnd = ::CreateWindowEx(
				dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
				);
			return (m_hWnd ? TRUE : FALSE);
		}
		BOOL Show(int nCmdShow = SW_SHOW)
		{
			assert(m_hWnd != NULL);
			return ::ShowWindow(m_hWnd, nCmdShow);
		}
		HWND GetHWND() const {
			return m_hWnd;
		}
		operator HWND() {
			return m_hWnd;
		}
		BOOL GetClientRect(LPRECT rc) {
			assert(m_hWnd != NULL);
			return ::GetClientRect(m_hWnd, rc);
		}
		void Invalidate() {
			assert(m_hWnd != NULL);
			RECT rc;
			GetClientRect(&rc);
			InvalidateRect(m_hWnd, &rc, TRUE);
		}
	protected:
		//functions must be implement
		virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
		virtual LPCTSTR ClassName() const = 0;
	public:
		//static method
		static LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			WndCls* pThis = NULL;
			if (uMsg == WM_NCCREATE) {
				LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				pThis = static_cast<WndCls*>(lpcs->lpCreateParams);
				pThis->m_hWnd = hWnd;
				::SetWindowLong(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
			}
			else {
				pThis = reinterpret_cast<WndCls*>(::GetWindowLong(hWnd, GWLP_USERDATA));
			}
			if (pThis != NULL) {
				return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
			}
			else {
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}
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
	public:
		//some default function
		virtual LPCTSTR GetDefaultCursor() const {
			return IDC_ARROW;
		}

	private:
		HWND m_hWnd;

	};
}
#endif //ZUI_ZUIBASE_HEADER