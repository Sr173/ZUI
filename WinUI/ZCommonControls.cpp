#include "stdafx.h"
#include "ZCommonControls.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
#include <assert.h>
namespace ZUI
{
	//ZLabel begin
	void ZLabel::DrawSelf(HWND owner, ZGdiplusManager* painter, const RECT& _rc)
	{
		assert(IsWindow(owner));
		if (!IsRectCross(_rc, m_rc)) return;
		Gdiplus::Graphics				graphics(owner);
		Gdiplus::Color backColor =		ZColor2GdiColor(m_backColor);
		Gdiplus::Color fontColor =		ZColor2GdiColor(m_fontColor);
		Gdiplus::Color borderColor =	ZColor2GdiColor(m_borderColor);
		Gdiplus::SolidBrush				backBrush(backColor);
		Gdiplus::Pen					borderPen(borderColor);
		Gdiplus::SolidBrush				fontBrush(fontColor);

		Gdiplus::RectF rc(static_cast<float>(m_rc.left), 
			static_cast<float>(m_rc.top),
			static_cast<float>(m_rc.right - m_rc.left),
			static_cast<float>(m_rc.bottom - m_rc.top));
		
		graphics.FillRectangle(&backBrush, rc);
		graphics.DrawRectangle(&borderPen, rc);
		if (!m_text.IsNull()) {
			/*
			float fontx = static_cast<float>((m_rc.left + m_rc.right) / 2 );
			float fonty = static_cast<float>((m_rc.top + m_rc.bottom) / 2);
			Gdiplus::PointF fontPt(fontx, fonty);
			*/
			StringFormat fontFormat;
			fontFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			fontFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			FontFamily fontFamily(L"Times New Roman");
			Font font(&fontFamily,
				static_cast<Gdiplus::REAL>(m_fontSize),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			graphics.DrawString(m_text, -1, &font, rc, &fontFormat, &fontBrush);
		}
	}
	void ZLabel::Release()
	{
		delete this;
	}
	//message
	LONG ZLabel::OnLButtonUp(ZControl* con, ZMouseState s)
	{
		if (PointInRect(s.x, s.y, m_rc)) {
			m_lbuttonUp.Invoke(con, s);
		}
		return 0;
	}
	LONG ZLabel::OnRButtonUp(ZControl* con, ZMouseState s)
	{
		if (PointInRect(s.x, s.y, m_rc)) {
			m_rbuttonUp.Invoke(con, s);
		}
		return 0;
	}
	LONG ZLabel::OnMouseMove(ZControl* con, ZMouseState s)
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
	//ZLabel end
	//ZButton begin
	ZButton::ZButton() :
		ZLabel(),
		m_hoverBackColor(127, 127, 127, 127),
		m_bHovered(false)
	{
		NotifyOnMouseMoveIn(ZButton::OnMouseInEvent);
		NotifyOnMouseMoveOut(ZButton::OnMouseOutEvent);
	}
	void ZButton::DrawSelf(HWND owner, ZGdiplusManager* painter, const RECT& _rc)
	{
		ZColor tmpColor = m_backColor;
		if (m_bHovered) {
			m_backColor = m_hoverBackColor;
		}
		ZLabel::DrawSelf(owner, painter, _rc);
		m_backColor = tmpColor;
	}
	LONG ZButton::OnMouseInEvent(ZControl* con, ZMouseState s)
	{
		ZButton* pThis = dynamic_cast<ZButton*>(con);
		pThis->m_bHovered = true;
		pThis->Invalidate();
		return 0;
	}
	LONG ZButton::OnMouseOutEvent(ZControl* con, ZMouseState s)
	{
		ZButton* pThis = dynamic_cast<ZButton*>(con);
		pThis->m_bHovered = false;
		pThis->Invalidate();
		return 0;
	}
	//ZButton end
	//ZTextBox begin
	ZTextBox::ZTextBox() :
		ZLabel()
	{
		SetTextColor(0, 0, 0, 255);
		SetBackColor(255, 255, 255, 255);
		NotifyOnLButtonUp(ClickAndChangeFocus);
	}
	void ZTextBox::SetFocus()
	{
		if (!m_bFocus) {
			m_bFocus = true;
			OnGetFocus();
		}
	}
	void ZTextBox::LostFocus()
	{
		if (m_bFocus) {
			m_bFocus = false;
			OnLostFocus();
		}
	}
	LONG ZTextBox::OnLButtonUp(ZControl* con, ZMouseState s)
	{
		m_lbuttonUp.Invoke(con, s);
		return 0;
	}
	LONG ZTextBox::ClickAndChangeFocus(ZControl* con, ZMouseState s)
	{
		ZTextBox* pThis = dynamic_cast<ZTextBox*>(con);
		if ( PointInRect(s.x, s.y, pThis->GetRect()) ) {
			pThis->SetFocus();
			return 1;
		}
		else {
			pThis->LostFocus();
			return -1;
		}
	}
	//ZTextBox end
}