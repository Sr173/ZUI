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
	inline ZRect RECT2ZRect(const RECT& rc)
	{
		ZRect zrc;
		zrc.bottom = rc.bottom;
		zrc.left = rc.left;
		zrc.right = rc.right;
		zrc.top = rc.top;
		return zrc;
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
		virtual const wchar_t* GetType() const {
			return L"control";
		}
	public:
		virtual void DrawSelf(HWND owner, ZRender* render, const ZRect& rc) = 0;
		virtual void HandleEvent(ZControlMsg& msg) = 0;
		virtual void Release() = 0;
	public:
		virtual void SetId(LPCTSTR id) {
			m_id = id;
		}
		virtual ZString GetId() const {
			return m_id;
		}
		virtual ZControl* FindControl(ZString id) {
			if ((!m_id.IsNull()) && m_id == id) {
				return this;
			}
			else {
				return nullptr;
			}
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
			m_hWnd(nullptr),
			m_paintManager(nullptr),
			m_render(nullptr),
			m_backColor(255,255,255)
		{}
		virtual ~ZWinBase()
		{
			for each (auto control in m_pControls)
			{
				control->Release();
			}
			if (m_render != nullptr) {
				m_render->Release();
			}
		}
		virtual const wchar_t* GetType() const {
			return L"winbase";
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
			m_render = GetRender();
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
		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
		{
			return ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
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
					//��װһ����Ϣ���û�����
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
			case WM_DISPLAYCHANGE:
				Invalidate();
				msg.bHandled = true;
				break;
			case WM_PAINT:
				::BeginPaint(msg.hWnd, &ps);
				DrawMySelf(ps.rcPaint);
				::EndPaint(msg.hWnd, &ps);
				msg.uMsg = ZUIM_PAINT;
				msg.bHandled = true;
				break;
			case WM_SIZE:
				RefreshRender();
				Invalidate();
				break;
			case WM_MOVE:
				break;
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
			ZRender* render = GetRender();
			render->BeginRender();
			render->Clear(m_backColor);
			ZRect zrc = RECT2ZRect(rc);
			for each (auto control in m_pControls)
			{
				control->DrawSelf(m_hWnd, render, zrc);
			}
			
			render->EndRender();
		}
		void AddControl(ZControl* control) {
			assert(control != NULL);
			control->SetHWND(m_hWnd);
			m_pControls.push_back(control);
		}
		ZControl* FindControl(ZString id)
		{
			for each (auto control in m_pControls)
			{
				ZControl* ret = control->FindControl(id);
				if (ret != nullptr) {
					return ret;
				}
			}
			return nullptr;
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
		void RefreshRender()
		{
			RECT rc;
			GetClientRect(&rc);
			ZSize size;
			size.width = rc.right - rc.left;
			size.height = rc.bottom - rc.top;
			m_render->ReSize(size);
		}
		ZRender* GetRender()
		{
			if (m_render == nullptr) {
				m_render = m_paintManager->CreateRender(m_hWnd);
			}
			return m_render;
		}
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
		ZPaintManager*			m_paintManager;
		ZRender*				m_render;
		ZColor					m_backColor;
	};
}
#endif //ZUI_ZUIBASE_HEADER