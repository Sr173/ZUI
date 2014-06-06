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

		Gdiplus::Rect rc(m_rc.left, m_rc.top,
				m_rc.right - m_rc.left, m_rc.bottom - m_rc.top);
		
		graphics.FillRectangle(&backBrush, rc);
		graphics.DrawRectangle(&borderPen, rc);
		if (!m_text.IsNull()) {
			float fontx = static_cast<float>((m_rc.left + m_rc.right) / 2 );
			float fonty = static_cast<float>((m_rc.top + m_rc.bottom) / 2);
			Gdiplus::PointF fontPt(fontx, fonty);
			StringFormat fontFormat;
			fontFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			fontFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
			FontFamily fontFamily(L"Times New Roman");
			Font font(&fontFamily,
				static_cast<Gdiplus::REAL>(m_fontSize),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			graphics.DrawString(m_text, -1, &font, fontPt, &fontFormat, &fontBrush);
		}
	}
	void ZLabel::Release()
	{
		delete this;
	}
}