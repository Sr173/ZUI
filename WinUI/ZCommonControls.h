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
	public:
		//ZControl methods
		virtual void DrawSelf(HWND owner, ZGdiplusManager* painter, const RECT& rc);
		virtual void Release();
	public:
		ZLabel&		SetText(const ZStringW& text) {
			m_text = text;
			return *this;
		}
		ZStringW	GetText() const {
			return m_text;
		}
		ZLabel&		SetTextSize(int size) {
			m_fontSize = size;
			return *this;
		}
		ZLabel&		SetTextColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_fontColor = ZColor(r, g, b, a);
			return *this;
		}
		ZLabel&		SetBackColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_backColor = ZColor(r, g, b, a);

			return *this;
		}
		ZLabel&		SetBorderColor(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
			m_borderColor = ZColor(r, g, b, a);
			return *this;
		}
		ZLabel&		SetRect(RECT rc) {
			m_rc = rc;
			return *this;
		}
		RECT		GetRect() const {
			return m_rc;
		}
		ZLabel&		SetWidth(int width) {
			m_rc.right = m_rc.left + width;
			return *this;
		}
		int			GetWidth() const {
			return m_rc.right - m_rc.left;
		}
		ZLabel&		SetHeight(int height) {
			m_rc.bottom = m_rc.top + height;
			return *this;
		}
		int			GetHeight() const {
			return m_rc.bottom - m_rc.top;
		}
		ZLabel&		SetPosition(int x, int y) {
			m_rc.right = GetWidth() + x;
			m_rc.bottom = GetHeight() + y;
			m_rc.left = x;
			m_rc.top = y;
			return *this;
		}
		void Invalidate()
		{
			assert(::IsWindow(m_parent));
			::InvalidateRect(m_parent, &m_rc, TRUE);
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
		virtual LONG OnLButtonUp(ZControl* con, ZMouseState s)
		{
			if (PointInRect(s.x, s.y, m_rc)) {
				m_lbuttonUp.Invoke(con, s);
			}
			return 0;
		}
		virtual LONG OnRButtonUp(ZControl* con, ZMouseState s)
		{
			if (PointInRect(s.x, s.y, m_rc)) {
				m_rbuttonUp.Invoke(con, s);
			}
			return 0;
		}
		virtual LONG OnMouseMove(ZControl* con, ZMouseState s)
		{
			if (PointInRect(s.x, s.y, m_rc)) {
				if (!m_bMouseIn) {
					m_bMouseIn = true;
					m_mouseMoveIn.Invoke(con, s);
				}
				m_mouseMove.Invoke(con, s);
			}
			else {
				if (m_bMouseIn) {
					m_bMouseIn = false;
					m_mouseMoveOut.Invoke(con, s);
				}
			}
			return 0;
		}
	protected:
		ZStringW	m_text;	//Label text
		int			m_fontSize;
		RECT		m_rc; //layout
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
}


#endif //ZUI_ZCOMMONCONTROLS_HEADER