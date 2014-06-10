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
	//Msg Struct
	typedef struct {
		HWND	hWnd;
		UINT	uMsg;
		WPARAM	wParam;
		LPARAM	lParam;
		bool	bHandled;
	} ZControlMsg, *pZControlMsg;
	inline ZControlMsg MakeZControlMsg(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam, bool bHandled = false)
	{
		ZControlMsg controlMsg;
		controlMsg.hWnd = hWnd; 
		controlMsg.uMsg = uMsg;
		controlMsg.wParam = wParam;
		controlMsg.lParam = lParam;
		controlMsg.bHandled = bHandled;
		return controlMsg;
	}
	//ZUI Message Define
	enum
	{
		ZUIM_PAINT = WM_USER + 100,

	};
	class ZControl :
		public ZObject
	{
	public:
		ZControl() :
			m_parent(NULL),
			m_bFocus(false)
		{
			m_id.SetNull(true);
		}
		virtual ~ZControl()
		{}
	public:
		virtual void DrawSelf(HWND owner, ZRender* render, const RECT& rc) = 0;
		virtual void HandleEvent(ZControlMsg& msg) = 0;
		virtual void Release() = 0;
	public:
		virtual void SetId(LPCTSTR id) {
			m_id = id;
		}
		virtual ZString GetId() const {
			return m_id;
		}
		virtual void SetHWND(HWND hwnd)
		{
			m_parent = hwnd;
		}
		virtual void SetFocus() {
			m_bFocus = true;
		}
		virtual void LostFocus() {
			m_bFocus = false;
		}
		virtual bool IsFocus() const {
			return m_bFocus;
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
		ZString		m_id;
		bool		m_bFocus;
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
				ZControlMsg controlMsg = MakeZControlMsg(
					hWnd, uMsg, wParam, lParam, false);
				pThis->PreHandleMessage(controlMsg);
				
				if (uMsg != WM_PAINT) {
					//user shouldn't paint
					//to do 
					//封装一个消息给用户处理
					//lret = pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
				}
				pThis->FinalHandleMessage(controlMsg);
				
				if (!controlMsg.bHandled) {
					lret = DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
			}
			else {
				lret = DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			return lret;
		}
		bool PreHandleMessage(ZControlMsg& msg)
		{
			ZMouseState s;
			PAINTSTRUCT ps;
			switch (msg.uMsg)
			{
			case WM_PAINT:
				::BeginPaint(msg.hWnd, &ps);
				DrawMySelf(ps.rcPaint);
				::EndPaint(msg.hWnd, &ps);
				msg.uMsg = ZUIM_PAINT;
				msg.bHandled = true;
				break;
			case WM_SIZE:
				m_paintManager->Invalidate();
				break;
			case WM_MOVE:
				m_paintManager->Invalidate();
			default:
				for each (auto control in m_pControls) {
					control->HandleEvent(msg);
				}
				break;
			}
			return true;
		}
		bool FinalHandleMessage(ZControlMsg& msg)
		{
			ZMouseState s;
			switch (msg.uMsg)
			{
			case WM_DESTROY:
				msg.bHandled = true;
				::PostQuitMessage(0);
				break;
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
			ZRender* render = m_paintManager->GetRender();
			for each (auto control in m_pControls)
			{
				control->DrawSelf(m_hWnd, render, rc);
			}
		}
		void AddControl(ZControl* control) {
			assert(control != NULL);
			control->SetHWND(m_hWnd);
			m_pControls.push_back(control);
		}
		bool RemoveControl(ZControl* control) {
			auto iter = FindControl(control);
			if (iter == m_pControls.end()) {
				return false;
			}
			else {
				m_pControls.erase(iter);
				control->Release();
				return false;
			}
		}
		void SetPaintManager(ZPaintManager* painter) {
			m_paintManager = painter;
		}
	protected:
		std::vector<ZControl*>::iterator FindControl(ZControl* control)
		{
			for (auto iter = m_pControls.begin();
				iter != m_pControls.end(); ++iter) {
				if (*iter == control) {
					return iter;
				}
			}
			return m_pControls.end();
		}
	protected:
		HWND					m_hWnd;
	private:
		std::vector<ZControl*>	m_pControls;
		ZPaintManager*		m_paintManager;
	};
}
#endif //ZUI_ZUIBASE_HEADER