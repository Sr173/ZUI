#ifndef ZUI_ZCOMMONCONTROLS_HEADER
#define ZUI_ZCOMMONCONTROLS_HEADER
#include "ZBase.h"
#include "ZUIBase.h"
#include "ZEvent.h"

namespace ZUI
{
	class ZLabel :
		public ZControl
	{
	public:
		ZLabel() :
			ZControl(),
			m_fontColor(0, 0, 0),
			m_backColor(255, 255, 255, 0),
			m_borderColor(0, 0, 0),
			m_fontSize(12),
			m_bMouseIn(false)
		{
			m_rc.left = 10; m_rc.top = 10;
			m_rc.right = 60; m_rc.bottom = 30;
		}
		virtual ~ZLabel()
		{}
		virtual const wchar_t* GetType() const {
			return L"label";
		}
	public:
		//ZControl methods
		virtual void DrawSelf(HWND owner, ZRender* render, const ZRect& rc);
		virtual void HandleEvent(ZControlMsg& msg);
		virtual void Release();
	public:
		virtual void		SetText(const ZStringW& text) {
			m_text = text;
		}
		virtual ZStringW	GetText() const {
			return m_text;
		}
		virtual void		SetTextSize(int size) {
			m_fontSize = size;
		}
		virtual void		SetTextColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_fontColor = ZColor(r, g, b, a);
		}
		virtual void		SetBackColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_backColor = ZColor(r, g, b, a);
		}
		virtual void		SetBorderColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_borderColor = ZColor(r, g, b, a);
		}
		void		SetRect(ZRect rc) {
			m_rc = rc;
		}
		ZRect		GetRect() const {
			return m_rc;
		}
		void		SetWidth(int width) {
			m_rc.right = m_rc.left + width;
		}
		int			GetWidth() const {
			return m_rc.right - m_rc.left;
		}
		void		SetHeight(int height) {
			m_rc.bottom = m_rc.top + height;
		}
		int			GetHeight() const {
			return m_rc.bottom - m_rc.top;
		}
		void		SetPosition(int x, int y) {
			m_rc.right = GetWidth() + x;
			m_rc.bottom = GetHeight() + y;
			m_rc.left = x;
			m_rc.top = y;
		}
		void Invalidate()
		{
			if (::IsWindow(m_parent)) {
				RECT rc;
				rc.left = m_rc.left;
				rc.right = m_rc.right;
				rc.bottom = m_rc.bottom;
				rc.top = m_rc.top;
				::InvalidateRect(m_parent, &rc, TRUE);
			}
		}
	public:
		//message function
		void NotifyOnLButtonUp(MouseFunc func) {
			m_lbuttonUp.subscribe(func);
		}
		void NotifyOnRButtonUp(MouseFunc func) {
			m_rbuttonUp.subscribe(func);
		}
		void NotifyOnMouseMove(MouseFunc func) {
			m_mouseMove.subscribe(func);
		}
		void NotifyOnMouseMoveIn(MouseFunc func) {
			m_mouseMoveIn.subscribe(func);
		}
		void NotifyOnMouseMoveOut(MouseFunc func) {
			m_mouseMoveOut.subscribe(func);
		}
		virtual LONG OnLButtonUp(ZControl* con, ZMouseState s);
		virtual LONG OnRButtonUp(ZControl* con, ZMouseState s);
		virtual LONG OnMouseMove(ZControl* con, ZMouseState s);
	protected:
		ZStringW	m_text;	//Label text
		int			m_fontSize;
		ZRect		m_rc; //layout
		ZColor		m_fontColor;
		ZColor		m_backColor;
		ZColor		m_borderColor;
		ZMouseEvent m_lbuttonUp;
		ZMouseEvent m_mouseMove;
		bool		m_bMouseIn;
		ZMouseEvent m_mouseMoveIn;
		ZMouseEvent m_mouseMoveOut;
		ZMouseEvent m_rbuttonUp;
	};
	class ZButton :
		public ZLabel
	{
	public:
		ZButton();
		virtual ~ZButton()
		{}
		virtual const wchar_t* GetType() const {
			return L"button";
		}
	public:
		virtual void DrawSelf(HWND owner, ZRender* render, const ZRect& _rc);
	public:
		void	SetHoverColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_hoverBackColor = ZColor(r, g, b, a);
		}
	protected:
		static LONG OnMouseInEvent(ZControl* con, ZMouseState s);
		static LONG OnMouseOutEvent(ZControl* con, ZMouseState s);
	protected:
		ZColor		m_hoverBackColor;
		bool		m_bHovered;
	};
	class ZTextBox :
		public ZLabel
	{
	public:
		ZTextBox();
		virtual ~ZTextBox()
		{}
		virtual const wchar_t* GetType() const {
			return L"textbox";
		}
	public:
		virtual void SetFocus();
		virtual void LostFocus();
		virtual void HandleEvent(ZControlMsg& msg);
		virtual void SetBackColor(BYTE r, BYTE g, BYTE b, BYTE a) {
			//TextBox ingore alpha
			ZLabel::SetBackColor(r, g, b, 255);
		}
	public:
		void NotifyOnGetFocus(ControlFunc func) {
			m_getFocus.subscribe(func);
		}
		void NotifyOnLostFocus(ControlFunc func) {
			m_lostFocus.subscribe(func);
		}
		virtual LONG OnGetFocus() {
			m_getFocus.Invoke(this, NULL);
			return 0;
		}
		virtual LONG OnLostFocus() {
			m_lostFocus.Invoke(this, NULL);
			return 0;
		}
		virtual LONG OnIMEInput(ZControlMsg& msg);
		virtual LONG OnCharInput(ZControlMsg& msg);
		virtual LONG OnLButtonUp(ZControl* con, ZMouseState s);
	protected:
		static LONG ClickAndChangeFocus(ZControl* con, ZMouseState s);
	protected:
		ZControlEvent	m_lostFocus;
		ZControlEvent	m_getFocus;
	};
}


#endif //ZUI_ZCOMMONCONTROLS_HEADER