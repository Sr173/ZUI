#include "stdafx.h"
#include "ZCommonControls.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Imm32.lib")
#include <assert.h>
namespace ZUI
{
	//ZLabel begin
	void ZLabel::DrawSelf(HWND owner, ZRender* render, const RECT& _rc)
	{
		assert(IsWindow(owner));
		//if (!IsRectCross(_rc, m_rc)) return;
		//ZRender* render = painter->CreateRender(owner, m_rc);
		if (render == nullptr) return;
		render->FillRectangle(m_rc, m_backColor);
		render->DrawRectangle(m_rc, m_borderColor);
		if (!m_text.IsNull()) {
			render->PaintText(m_rc, m_text, m_fontSize, m_fontColor, ZRender::TAMMiddle, ZRender::TAMMiddle);
		}
	}
	void ZLabel::HandleEvent(ZControlMsg& msg)
	{
		ZMouseState mouseState(msg.wParam, msg.lParam);
		switch (msg.uMsg)
		{
		case WM_LBUTTONUP:
			msg.bHandled = true;
			OnLButtonUp(this, mouseState);
			break;
		case WM_RBUTTONUP:
			msg.bHandled = true;
			OnRButtonUp(this, mouseState);
			break;
		case WM_MOUSEMOVE:
			msg.bHandled = true;
			OnMouseMove(this, mouseState);
		default:
			break;
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
	void ZButton::DrawSelf(HWND owner, ZRender* render, const RECT& _rc)
	{
		ZColor tmpColor = m_backColor;
		if (m_bHovered) {
			m_backColor = m_hoverBackColor;
		}
		ZLabel::DrawSelf(owner, render, _rc);
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
		SetWidth(200);
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
	void ZTextBox::HandleEvent(ZControlMsg& msg)
	{
		switch (msg.uMsg)
		{
		case WM_IME_COMPOSITION:
			OnIMEInput(msg);
			break;
		case WM_CHAR:
			OnCharInput(msg);
			break;
		default:
			ZLabel::HandleEvent(msg);
			break;
		}
	}
	LONG ZTextBox::OnIMEInput(ZControlMsg& msg)
	{
		HIMC hIMC;
		DWORD dwSize;
		LPWSTR lpstr;
		if (IsFocus()) {
			if (msg.lParam & GCS_RESULTSTR)
			{
				hIMC = ImmGetContext(msg.hWnd);
				dwSize = ImmGetCompositionString(
					hIMC, GCS_RESULTSTR, NULL, 0);
				dwSize += sizeof(WCHAR);
				lpstr = new WCHAR[dwSize];
				ImmGetCompositionString(
					hIMC, GCS_RESULTSTR, lpstr, dwSize);
				ImmReleaseContext(msg.hWnd, hIMC);
				lpstr[dwSize / sizeof(_TCHAR)-1] = 0;
				ZStringW conText = GetText();
				conText.Append(lpstr);
				delete[] lpstr;
				SetText(conText);
				Invalidate();
				msg.bHandled = true;
			}
		}
		return 0;
	}
	LONG ZTextBox::OnCharInput(ZControlMsg& msg)
	{
		WCHAR keyChar;
		if (IsFocus()) {
			if (msg.lParam & (1 << 24)) return -1;
			keyChar = static_cast<WCHAR>(msg.wParam);
			ZStringW conText = GetText();
			conText.Append(keyChar);
			SetText(conText);
			Invalidate();
			msg.bHandled = true;
		}
		return 0;
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