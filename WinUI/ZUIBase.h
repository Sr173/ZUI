#ifndef ZUI_ZUIBASE_HEADER
#define ZUI_ZUIBASE_HEADER
#include "ZBase.h"
#include "ZPaintManager.h"
#include "ZEvent.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <assert.h>
namespace ZUI
{
	class ZControl :
		public ZObject
	{
	public:
		ZControl() :
			m_parent(NULL)
		{}
		virtual ~ZControl()
		{}
	public:
		virtual void DrawSelf(HWND owner, ZGdiplusManager* painter, const RECT& rc) = 0;
		virtual void Release() = 0;
	public:
		void SetHWND(HWND hwnd)
		{
			m_parent = hwnd;
		}
	public:
		//message function
		//Mouse Message
		virtual LONG OnLButtonUp(ZControl* con, ZMouseState s)
		{
			return 0;
		}
		virtual LONG OnMouseMove(ZControl* con, ZMouseState s)
		{
			return 0;
		}
		virtual LONG OnRButtonUp(ZControl* con, ZMouseState s)
		{
			return 0;
		}
	protected:
		HWND		m_parent;
	};

	template <class WndCls>
	class ZWinBase :
		public ZObject
	{
	public:
		ZWinBase() :
			m_hWnd(NULL),
			m_paintManager(NULL)
		{}
		virtual ~ZWinBase()
		{
			for each (auto control in m_pControls)
			{
				control->Release();
			}
		}
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
			if (m_hWnd) {
				for each (auto control in m_pControls) {
					control->SetHWND(m_hWnd);
				}
			}
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
		BOOL GetWindowRect(LPRECT rc) {
			assert(m_hWnd != NULL);
			return ::GetWindowRect(m_hWnd, rc);
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
			LRESULT lret = 0;
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
				pThis->PreHandleMessage(hWnd, uMsg, wParam, lParam);
				
				if (uMsg != WM_PAINT) {
					//user shouldn't paint
					//to do 
					//封装一个消息给用户处理
					lret = pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
				}
				pThis->FinalHandleMessage(hWnd, uMsg, wParam, lParam);
			}
			else {
				lret = DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return lret;
		}
		bool PreHandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			ZMouseState s;
			PAINTSTRUCT ps;
			switch (uMsg)
			{
			case WM_PAINT:
				::BeginPaint(hWnd, &ps);
				::EndPaint(hWnd, &ps);
				DrawMySelf(ps.rcPaint);
				break;
			case WM_MOUSEMOVE:
				break;
			default:
				break;
			}
			return true;
		}
		bool FinalHandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			ZMouseState s;
			switch (uMsg)
			{
			case WM_LBUTTONUP:
				s = ZMouseState(wParam, lParam);
				for each (auto control in m_pControls) {
					control->OnLButtonUp(control, s);
				}
				break;
			case WM_RBUTTONUP:
				s = ZMouseState(wParam, lParam);
				for each (auto control in m_pControls) {
					control->OnRButtonUp(control, s);
				}
				break;
			case WM_MOUSEMOVE:
				s = ZMouseState(wParam, lParam);
				for each (auto control in m_pControls) {
					control->OnMouseMove(control, s);
				}
			default:
				break;
			}
			return true;
		}
	public:
		//some default function
		virtual LPCTSTR GetDefaultCursor() const {
			return IDC_ARROW;
		}
		void DrawMySelf(const RECT& rc) {
			assert(m_paintManager != NULL);
			for each (auto control in m_pControls)
			{
				control->DrawSelf(m_hWnd, m_paintManager, rc);
			}
		}
		void AddControl(ZControl* control) {
			assert(control != NULL);
			control->SetHWND(m_hWnd);
			m_pControls.push_back(control);
		}
		void SetPaintManager(ZGdiplusManager* painter) {
			m_paintManager = painter;
		}
	protected:
		HWND					m_hWnd;
	private:
		std::vector<ZControl*>	m_pControls;
		ZGdiplusManager*		m_paintManager;
	};
}
#endif //ZUI_ZUIBASE_HEADER