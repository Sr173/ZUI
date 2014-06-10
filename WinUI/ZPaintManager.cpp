#include "stdafx.h"
#include "ZPaintManager.h"
#include <assert.h>
#include <atlbase.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
namespace ZUI
{
	namespace
	{
		Gdiplus::StringAlignment TextAlign2GdiAlign(ZRender::TextAlignMode mode)
		{
			switch (mode)
			{
			case ZRender::TAMNear:
				return Gdiplus::StringAlignment::StringAlignmentNear;
			case ZRender::TAMMiddle:
				return Gdiplus::StringAlignment::StringAlignmentCenter;
			case ZRender::TAMFar:
				return Gdiplus::StringAlignment::StringAlignmentFar;
			default:
				return Gdiplus::StringAlignment::StringAlignmentCenter;
			}
		}
		DWRITE_TEXT_ALIGNMENT TextAlign2DirectAlign(ZRender::TextAlignMode mode)
		{
			switch (mode)
			{
			case ZRender::TAMNear:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
			case ZRender::TAMMiddle:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
			case ZRender::TAMFar:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
			default:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
			}
		}
		DWRITE_PARAGRAPH_ALIGNMENT TextAlign2DirectParaphAlign(ZRender::TextAlignMode mode)
		{
			switch (mode)
			{
			case ZRender::TAMNear:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
			case ZRender::TAMMiddle:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			case ZRender::TAMFar:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
			default:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
		D2D1::ColorF ZColor2DirectColor(ZColor color)
		{
			D2D1::ColorF D2DColor(
				static_cast<FLOAT>(color.r) / 255.f,
				static_cast<FLOAT>(color.g) / 255.f,
				static_cast<FLOAT>(color.b) / 255.f,
				static_cast<FLOAT>(color.a) / 255.f
				);
			return D2DColor;
		}
	}
	class ZGdiRender :
		public ZRender
	{
	public:
		ZGdiRender(HWND hWnd, const RECT& rc) :
			m_hWnd(hWnd), m_graphics(hWnd),
			m_rc(rc)
		{
		}
		virtual ~ZGdiRender()
		{}
	public:
		//ZRender method
		virtual bool PaintText(RECT rc, ZStringW text,
			int fontSize, ZColor fontColor, TextAlignMode alignModeX, TextAlignMode alignModeY);
		virtual bool FillRectangle(RECT rc, ZColor color);
		virtual bool DrawRectangle(RECT rc, ZColor color);
		virtual bool DrawLine(ZColor color, POINT pt1, POINT pt2);
		virtual bool Clear(ZColor color);
		virtual void Release()
		{
			delete this;
		}
	private:
		HWND				m_hWnd;
		RECT				m_rc;
		Gdiplus::Graphics	m_graphics;
	};
	class ZDirectRender :
		public ZRender
	{
	public:
		ZDirectRender(HWND hWnd, const RECT& rc) :
			m_hWnd(hWnd), m_rc(rc)
		{
			CreateResource();
		}
		virtual ~ZDirectRender()
		{}
	public:
		//ZRender method
		virtual bool PaintText(RECT rc, ZStringW text,
			int fontSize, ZColor fontColor, TextAlignMode alignModeX, TextAlignMode alignModeY);
		virtual bool FillRectangle(RECT rc, ZColor color);
		virtual bool DrawRectangle(RECT rc, ZColor color);
		virtual bool DrawLine(ZColor color, POINT pt1, POINT pt2);
		virtual bool Clear(ZColor color);
		virtual void Release()
		{
			delete this;
		}
	private:
		void CreateResource();
	private:
		CComPtr<ID2D1Factory>				m_pD2DFactory;
		CComPtr<IDWriteFactory>				m_pDWriteFactory;
		CComPtr<ID2D1HwndRenderTarget>		m_pRT;
		HWND								m_hWnd;
		RECT								m_rc;
	};

	bool ZPaintManager::Setup(ZString engine)
	{
		if (engine.ToLower() == _T("gdiplus")) {
			return Setup(RMGDIPlus);
		}
		else if (engine.ToLower() == _T("direct2d")) {
			return Setup(RMDirect2D);
		}
		return false;
	}
	ZRender* ZPaintManager::CreateRender()
	{
		ZRender* ret = nullptr;
		switch (m_mode)
		{
		case RMGDIPlus:
			ret = CreateGdiRender(m_hWnd);
			break;
		case RMDirect2D:
			ret = CreateDirectRender(m_hWnd);
			break;
		}
		return ret;
	}
	bool ZPaintManager::ResouceInit()
	{
		bool ret = false;
		switch (m_mode)
		{
		case RMGDIPlus:
			ret = GdiResourceInit();
			break;
		case RMDirect2D:
			ret = DX2DResourceInit();
			break;
		}
		return ret;
	}

	void ZPaintManager::Clear()
	{
		if (m_render != nullptr) {
			m_render->Release();
			m_render = nullptr;
		}
		switch (m_mode)
		{
		case RMGDIPlus:
			GdiClear();
			break;
		case RMDirect2D:
			DX2DClear();
			break;
		}
	}
	bool ZPaintManager::GdiResourceInit()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		return true;
	}
	ZRender* ZPaintManager::CreateGdiRender(HWND hWnd)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		ZGdiRender* pgdiRender = new ZGdiRender(hWnd, rc);
		return pgdiRender;
	}
	void ZPaintManager::GdiClear()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	bool ZPaintManager::DX2DResourceInit()
	{
		return true;
	}
	ZRender* ZPaintManager::CreateDirectRender(HWND hWnd)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		ZDirectRender* pdxRender = new ZDirectRender(hWnd, rc);
		return pdxRender;
	}
	void ZPaintManager::DX2DClear()
	{
	}
	//ZGdiRender begin

	bool ZGdiRender::PaintText(RECT rc, ZStringW text,
		int fontSize, ZColor fontColor, TextAlignMode alignModeX, TextAlignMode alignModeY)
	{
		Gdiplus::Color color = ZColor2GdiColor(fontColor);
		Gdiplus::SolidBrush fontBrush(color);
		Gdiplus::RectF rcF(static_cast<float>(rc.left),
			static_cast<float>(rc.top),
			static_cast<float>(rc.right - rc.left),
			static_cast<float>(rc.bottom - rc.top));
		Gdiplus::StringFormat fontFormat;
		fontFormat.SetAlignment(TextAlign2GdiAlign(alignModeX));
		fontFormat.SetLineAlignment(TextAlign2GdiAlign(alignModeY));
		Gdiplus::FontFamily fontFamily(L"Times New Roman");
		Gdiplus::Font font(&fontFamily,
			static_cast<Gdiplus::REAL>(fontSize),
			Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		m_graphics.DrawString(text.c_str(), -1, &font, rcF, &fontFormat, &fontBrush);
		return true;
	}
	bool ZGdiRender::FillRectangle(RECT rc, ZColor color)
	{
		Gdiplus::RectF rcF(static_cast<float>(rc.left),
			static_cast<float>(rc.top),
			static_cast<float>(rc.right - rc.left),
			static_cast<float>(rc.bottom - rc.top));
		Gdiplus::Color backColor = ZColor2GdiColor(color);
		Gdiplus::SolidBrush brush(backColor);
		m_graphics.FillRectangle(&brush, rcF);
		return true;
	}
	bool ZGdiRender::DrawRectangle(RECT rc, ZColor color)
	{
		Gdiplus::RectF rcF(static_cast<float>(rc.left),
			static_cast<float>(rc.top),
			static_cast<float>(rc.right - rc.left),
			static_cast<float>(rc.bottom - rc.top));
		Gdiplus::Color borderColor = ZColor2GdiColor(color);
		Gdiplus::Pen pen(borderColor);
		m_graphics.DrawRectangle(&pen, rcF);
		return true;
	}
	bool ZGdiRender::DrawLine(ZColor color, POINT pt1, POINT pt2)
	{
		Gdiplus::PointF ptstart(static_cast<Gdiplus::REAL>(pt1.x),
			static_cast<Gdiplus::REAL>(pt1.y));
		Gdiplus::PointF ptend(static_cast<Gdiplus::REAL>(pt2.x),
			static_cast<Gdiplus::REAL>(pt2.y));
		Gdiplus::Pen pen(ZColor2GdiColor(color));
		m_graphics.DrawLine(&pen, ptstart, ptend);
		return true;
	}
	bool ZGdiRender::Clear(ZColor color)
	{
		m_graphics.Clear(ZColor2GdiColor(color));
		return true;
	}
	//ZGdiRender end
	//ZDirectRender begin
	bool ZDirectRender::PaintText(RECT rc, ZStringW text, int fontSize, ZColor fontColor,
		TextAlignMode alignModeX, TextAlignMode alignModeY)
	{
		HRESULT hr = S_OK;
		CComPtr<ID2D1SolidColorBrush> pBrush;
		CComPtr<IDWriteTextFormat> pTextFormat;
		UINT32 cTextLength = static_cast<UINT32>(text.Length());
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			static_cast<FLOAT>(fontSize),
			L"zh-cn",
			&pTextFormat);
		if (FAILED(hr)) return false;
		pTextFormat->SetTextAlignment(TextAlign2DirectAlign(alignModeX));
		pTextFormat->SetParagraphAlignment(TextAlign2DirectParaphAlign(alignModeY));

		D2D1::ColorF brushColor = ZColor2DirectColor(fontColor);
		hr = m_pRT->CreateSolidColorBrush(
			brushColor,
			&pBrush);
		if (FAILED(hr)) return false;

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left),
			static_cast<FLOAT>(rc.top),
			static_cast<FLOAT>(rc.right),
			static_cast<FLOAT>(rc.bottom));
		m_pRT->BeginDraw();
		m_pRT->DrawTextW(
			text.c_str(),
			cTextLength,
			pTextFormat,
			layoutRect,
			pBrush);
		m_pRT->EndDraw();
		return true;
	}
	bool ZDirectRender::FillRectangle(RECT rc, ZColor color)
	{
		HRESULT hr = S_OK;
		CComPtr<ID2D1SolidColorBrush> pBrush;
		hr = m_pRT->CreateSolidColorBrush(
			ZColor2DirectColor(color),
			&pBrush);
		if (FAILED(hr)) return false;
		m_pRT->BeginDraw();
		m_pRT->FillRectangle(
			D2D1::RectF(
			static_cast<FLOAT>(rc.left),
			static_cast<FLOAT>(rc.top),
			static_cast<FLOAT>(rc.right),
			static_cast<FLOAT>(rc.bottom)),
			pBrush);
		m_pRT->EndDraw();
		return true;
	}
	bool ZDirectRender::DrawRectangle(RECT rc, ZColor color)
	{
		HRESULT hr = S_OK;
		CComPtr<ID2D1SolidColorBrush> pBrush;
		hr = m_pRT->CreateSolidColorBrush(
			ZColor2DirectColor(color),
			&pBrush);
		if (FAILED(hr)) return false;
		m_pRT->BeginDraw();
		m_pRT->DrawRectangle(
			D2D1::RectF(
			static_cast<FLOAT>(rc.left),
			static_cast<FLOAT>(rc.top),
			static_cast<FLOAT>(rc.right),
			static_cast<FLOAT>(rc.bottom)),
			pBrush);
		m_pRT->EndDraw();
		return true;
	}
	bool ZDirectRender::DrawLine(ZColor color, POINT pt1, POINT pt2)
	{
		HRESULT hr = S_OK;
		CComPtr<ID2D1SolidColorBrush> pBrush;
		hr = m_pRT->CreateSolidColorBrush(
			ZColor2DirectColor(color),
			&pBrush);
		if (FAILED(hr)) return false;
		m_pRT->BeginDraw();
		m_pRT->DrawLine(
			D2D1::Point2F(
			static_cast<FLOAT>(pt1.x),
			static_cast<FLOAT>(pt1.y)
			),
			D2D1::Point2F(
			static_cast<FLOAT>(pt2.x),
			static_cast<FLOAT>(pt2.y)
			),
			pBrush);
		m_pRT->EndDraw();
		return true;
	}
	bool ZDirectRender::Clear(ZColor color)
	{
		m_pRT->BeginDraw();
		m_pRT->Clear(ZColor2DirectColor(color));
		m_pRT->EndDraw();
		return true;
	}
	void ZDirectRender::CreateResource()
	{
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pD2DFactory);
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
		//RECT _rc;
		//::GetClientRect(m_hWnd, &_rc);
		D2D1_SIZE_U size = D2D1::SizeU(m_rc.right, m_rc.bottom);
		m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRT);
	}
	//ZDirectRender end
}