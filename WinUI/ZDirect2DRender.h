#ifndef ZUI_ZDIRECT2DRENDER_HEADER
#define ZUI_ZDIRECT2DRENDER_HEADER
#include <Windows.h>
#include <atlbase.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
#include <wincodec.h>
#pragma comment(lib, "Windowscodecs.lib")

#include "ZRender.h"
namespace ZUI
{
	namespace
	{
		D2D1_COLOR_F ZColorToColorF(ZColor color)
		{
			return D2D1::ColorF(
				static_cast<FLOAT>(color.r) / 255.f,
				static_cast<FLOAT>(color.g) / 255.f,
				static_cast<FLOAT>(color.b) / 255.f,
				static_cast<FLOAT>(color.a) / 255.f
				);
		}
		ZColor ColorFToZColor(D2D1_COLOR_F color)
		{
			return ZColor(
				static_cast<BYTE>(color.r * 255.f),
				static_cast<BYTE>(color.g * 255.f),
				static_cast<BYTE>(color.b * 255.f),
				static_cast<BYTE>(color.a * 255.f)
				);
		}
		D2D1_POINT_2F ZPointToPointF(ZPoint pt)
		{
			return D2D1::Point2F(
				static_cast<FLOAT>(pt.x),
				static_cast<FLOAT>(pt.y));
		}
		D2D1_RECT_F ZRectToRectF(ZRect rc)
		{
			return D2D1::RectF(
				static_cast<FLOAT>(rc.left),
				static_cast<FLOAT>(rc.top),
				static_cast<FLOAT>(rc.right),
				static_cast<FLOAT>(rc.bottom));
		}
		D2D1_SIZE_F ZSizeToSizeF(ZSize size)
		{
			return D2D1::SizeF(
				static_cast<FLOAT>(size.width),
				static_cast<FLOAT>(size.height));
		}
		D2D1_SIZE_U ZSizeToSizeU(ZSize size)
		{
			return D2D1::SizeU(
				static_cast<UINT32>(size.width),
				static_cast<UINT32>(size.height));
		}
		ZSize SizeFToZSize(D2D1_SIZE_F size)
		{
			ZSize tmp;
			tmp.height = static_cast<long>(size.height);
			tmp.width = static_cast<long>(size.width);
			return tmp;
		}
		DWRITE_TEXT_ALIGNMENT TextAlign2DirectAlign(ZFontFormat::TextAlignMode mode)
		{
			switch (mode)
			{
			case ZFontFormat::TAMNear:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
			case ZFontFormat::TAMMiddle:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
			case ZFontFormat::TAMFar:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
			default:
				return DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
			}
		}
		DWRITE_PARAGRAPH_ALIGNMENT TextAlign2DirectParaphAlign(ZFontFormat::TextAlignMode mode)
		{
			switch (mode)
			{
			case ZFontFormat::TAMNear:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
			case ZFontFormat::TAMMiddle:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			case ZFontFormat::TAMFar:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
			default:
				return DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			}
		}
	}
	class ZD2DBrush :
		public ZBrush
	{
	public:
		virtual ~ZD2DBrush()
		{}
	public:
		virtual ID2D1Brush* GetBrush() const = 0;
	};
	class ZD2DSolidBrush :
		public ZD2DBrush,
		public ZSolidBrush
	{
	public:
		ZD2DSolidBrush(CComPtr<ID2D1SolidColorBrush> brush) :
			m_pBrush(brush)
		{}
		virtual ~ZD2DSolidBrush()
		{}
	public:
		virtual const wchar_t* GetType() const {
			return L"d2dsolidbrush";
		}
		//ZBrush
		virtual void Release()
		{
			delete this;
		}
		//ZSolidBrush
		virtual ZColor GetColor() const {
			return ColorFToZColor(m_pBrush->GetColor());
		}
		virtual void SetColor(ZColor color)
		{
			m_pBrush->SetColor(ZColorToColorF(color));
		}
		//ZD2DBrush
		virtual ID2D1Brush* GetBrush() const {
			return m_pBrush;
		}
	private:
		CComPtr<ID2D1SolidColorBrush>	m_pBrush;
	};
	class ZD2DFontFormat :
		public ZFontFormat
	{
	public:
		ZD2DFontFormat(IDWriteTextFormat* textformat) :
			m_textFormat(textformat)
		{}
		virtual ~ZD2DFontFormat()
		{}
		virtual void Release()
		{
			delete this;
		}
	public:
		virtual void SetTextAlignment(TextAlignMode mode)
		{
			m_textFormat->SetTextAlignment(TextAlign2DirectAlign(mode));
		}
		virtual void SetParagraphAlignment(TextAlignMode mode)
		{
			m_textFormat->SetParagraphAlignment(TextAlign2DirectParaphAlign(mode));
		}
	public:
		IDWriteTextFormat* GetTextFormat() const{
			return m_textFormat;
		}
	private:
		CComPtr<IDWriteTextFormat>	m_textFormat;
	};
	class ZDirect2DBitmap :
		public ZBitmap
	{
	public:
		ZDirect2DBitmap(CComPtr<ID2D1Bitmap> pBitmap) :
			m_pBitmap(pBitmap)
		{
		}
		virtual ~ZDirect2DBitmap()
		{
			m_pBitmap.Release();
		}
	public:
		virtual ZSize GetSize() const
		{
			return SizeFToZSize(m_pBitmap->GetSize());
		}
		virtual void Release()
		{
			delete this;
		}
	public:
		ID2D1Bitmap* GetBitmap()
		{
			return m_pBitmap;
		}
	private:
		CComPtr<ID2D1Bitmap> m_pBitmap;
	};

	class ZDirect2DRender :
		public ZRender
	{
	public:
		ZDirect2DRender(HWND hWnd, ZSize size) :
			m_hWnd(hWnd), m_cPushLayer(0)
		{
			::CoInitialize(NULL);
			m_size.width = static_cast<UINT32>(size.width);
			m_size.height = static_cast<UINT32>(size.height);
			InitFactoryResource();
			InitTargetResource();
		}
		virtual ~ZDirect2DRender()
		{
			ClearAllResource();
			::CoUninitialize();
		}
	public:
		virtual ZRenderResult	BeginRender()
		{
			m_pRT->BeginDraw();
			return ZRENDER_OK;
		}
		virtual ZRenderResult	EndRender()
		{
			HRESULT hr = m_pRT->EndDraw();
			if (hr == D2DERR_RECREATE_TARGET) {
				DiscardTargetResource();
				InitTargetResource();
				hr = S_OK;
			}
			if (FAILED(hr)) {
				return ZRENDER_FAIL;
			}
			else {
				return ZRENDER_OK;
			}
		}
	public:
		virtual ZRenderResult	CreateSolidBrush(ZColor color, ZSolidBrush** pBrush)
		{
			CComPtr<ID2D1SolidColorBrush> pRet;
			HRESULT hr = m_pRT->CreateSolidColorBrush(
				ZColorToColorF(color),
				&pRet);
			if (FAILED(hr)) {
				return ZRENDER_FAIL;
			}
			else {
				*pBrush = new ZD2DSolidBrush(pRet);
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	CreateFontFormat(ZStringW fontFamily, long fontSize, ZFontFormat** ppFontFormat)
		{
			CComPtr<IDWriteTextFormat> textFormat;
			HRESULT hr = m_pDWriteFactory->CreateTextFormat(
				fontFamily.c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				static_cast<FLOAT>(fontSize),
				L"zh-cn",
				&textFormat);
			if (FAILED(hr)) {
				*ppFontFormat = nullptr;
				return ZRENDER_FAIL;
			}
			else {
				*ppFontFormat = new ZD2DFontFormat(textFormat);
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	CreateBitmap(ZStringW imageFilePath, ZBitmap** ppBitmap)
		{
			CComPtr<ID2D1Bitmap> pBitmap;
			HRESULT hr = LoadBitmapFromFile(m_pRT, imageFilePath, &pBitmap);
			if (FAILED(hr)) {
				return ZRENDER_FAIL;
			}
			else {
				ZDirect2DBitmap* pZBitmap = new ZDirect2DBitmap(pBitmap);
				*ppBitmap = pZBitmap;
				return ZRENDER_OK;
			}
		}
	public:
		virtual ZRenderResult	DrawRectangle(ZRect rc, ZBrush* pBrush)
		{
			ZD2DBrush* pD2DBrush = dynamic_cast<ZD2DBrush*>(pBrush);
			if (pD2DBrush == nullptr) {
				return ZRENDER_BAD_PARAM;
			}
			else {
				m_pRT->DrawRectangle(
					ZRectToRectF(rc),
					pD2DBrush->GetBrush());
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	DrawLine(ZPoint pt1, ZPoint pt2, ZBrush* pBrush)
		{
			ZD2DBrush* pD2DBrush = dynamic_cast<ZD2DBrush*>(pBrush);
			if (pD2DBrush == nullptr) {
				return ZRENDER_BAD_PARAM;
			}
			else {
				m_pRT->DrawLine(
					ZPointToPointF(pt1),
					ZPointToPointF(pt2),
					pD2DBrush->GetBrush());
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	FillRectangle(ZRect rc, ZBrush* pBrush)
		{
			ZD2DBrush* pD2DBrush = dynamic_cast<ZD2DBrush*>(pBrush);
			if (pD2DBrush == nullptr) {
				return ZRENDER_BAD_PARAM;
			}
			else {
				m_pRT->FillRectangle(
					ZRectToRectF(rc),
					pD2DBrush->GetBrush());
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	DrawEllipse(ZPoint pt, long rx, long ry, ZBrush* pBrush)
		{
			ZD2DBrush* pD2DBrush = dynamic_cast<ZD2DBrush*>(pBrush);
			if (pD2DBrush == nullptr) {
				return ZRENDER_BAD_PARAM;
			}
			else {
				D2D1_ELLIPSE ellipse;
				ellipse.point = ZPointToPointF(pt);
				ellipse.radiusX = static_cast<FLOAT>(rx);
				ellipse.radiusY = static_cast<FLOAT>(ry);
				m_pRT->DrawEllipse(ellipse, pD2DBrush->GetBrush());
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	PaintText(ZStringW text, ZRect rc, ZFontFormat* format, ZBrush* pBrush)
		{
			ZD2DBrush* pD2DBrush = dynamic_cast<ZD2DBrush*>(pBrush);
			ZD2DFontFormat* pD2DFormat = dynamic_cast<ZD2DFontFormat*>(format);
			if (pD2DBrush == nullptr || pD2DFormat == nullptr) {
				return ZRENDER_BAD_PARAM;
			}
			else {
				m_pRT->DrawTextW(text.c_str(), text.Length(),
					pD2DFormat->GetTextFormat(), ZRectToRectF(rc),
					pD2DBrush->GetBrush());
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	PaintImage(ZBitmap* pBitmap, const ZRect& rc, BYTE opacity)
		{
			ZDirect2DBitmap* pD2DBitmap = dynamic_cast<ZDirect2DBitmap*>(pBitmap);
			if (pD2DBitmap == nullptr) {
				return ZRENDER_FAIL;
			}
			m_pRT->DrawBitmap(
				pD2DBitmap->GetBitmap(),
				ZRectToRectF(rc),
				static_cast<FLOAT>(opacity) / 255.f
				);
			return ZRENDER_OK;
		}
		virtual ZRenderResult	Clear(ZColor color)
		{
			m_pRT->Clear(ZColorToColorF(color));
			return ZRENDER_OK;
		}
	public:
		virtual ZRenderResult	ReSize(ZSize size)
		{
			m_size = ZSizeToSizeU(size);
			HRESULT hr = m_pRT->Resize(D2D1::SizeU(
				static_cast<UINT32>(size.width),
				static_cast<UINT32>(size.height))
				);
			if (FAILED(hr)) {
				return ZRENDER_FAIL;
			}
			else {
				return ZRENDER_OK;
			}
		}
		virtual ZRenderResult	PushLayer(const ZRect& rc)
		{
			CComPtr<ID2D1Layer> pLayer;
			HRESULT hr = m_pRT->CreateLayer(&pLayer);
			if (FAILED(hr)) return ZRENDER_FAIL;
			m_pRT->PushLayer(D2D1::LayerParameters(
				ZRectToRectF(rc)), pLayer);
			++m_cPushLayer;
			return ZRENDER_OK;
		}
		virtual ZRenderResult	PopLayer()
		{
			if (m_cPushLayer == 0) {
				return ZRENDER_FAIL;
			}
			m_pRT->PopLayer();
			--m_cPushLayer;
			return ZRENDER_OK;
		}
		virtual void			Release() {
			delete this;
		}
	public:

	private:
		void InitFactoryResource()
		{
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				&m_pD2DFactory);
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
			HRESULT hr = m_pWICImageFactory.CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL);
			
		}
		void InitTargetResource()
		{
			m_pD2DFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hWnd, m_size),
				&m_pRT);
		}
		void ClearAllResource()
		{
			m_pRT.Release();
			m_pD2DFactory.Release();
			m_pDWriteFactory.Release();
			m_pWICImageFactory.Release();
		}
		void DiscardTargetResource()
		{
			m_pRT.Release();
		}
	private:
		HRESULT LoadBitmapFromDecoder(CComPtr<ID2D1HwndRenderTarget> target,
			CComPtr<IWICBitmapDecoder> pDecoder, ID2D1Bitmap** ppBitmap)
		{
			HRESULT hr = S_OK;
			CComPtr<IWICBitmapFrameDecode> pSource;
			CComPtr<IWICFormatConverter> pConverter;
			CComPtr<IWICBitmapScaler> pScaler;
			hr = pDecoder->GetFrame(0, &pSource);
			if (FAILED(hr)) {
				return hr;
			}
			hr = m_pWICImageFactory->CreateFormatConverter(&pConverter);
			if (FAILED(hr)) {
				return hr;
			}
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut);
			if (FAILED(hr)) {
				return hr;
			}
			hr = target->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				ppBitmap);
			return hr;
		}
		HRESULT LoadBitmapFromFile(CComPtr<ID2D1HwndRenderTarget> target,
			ZStringW imagePath, ID2D1Bitmap** ppBitmap)
		{
			HRESULT hr = S_OK;
			CComPtr<IWICBitmapDecoder> pDecoder;
			hr = m_pWICImageFactory->CreateDecoderFromFilename(
				imagePath.c_str(),
				NULL, GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&pDecoder);
			if (FAILED(hr)) {
				return hr;
			}
			return LoadBitmapFromDecoder(target, pDecoder, ppBitmap);
		}
		HRESULT LoadBitmapFromResource(CComPtr<ID2D1HwndRenderTarget> target,
			ZStringW resourceName, ZStringW resourceType, ID2D1Bitmap** ppBitmap)
		{
			CComPtr<IWICBitmapDecoder> pDecoder;
			CComPtr<IWICStream> pStream;
			HRSRC imageResHandle = NULL;
			HGLOBAL imageResDataHandle = NULL;
			void* pImageFile = NULL;
			DWORD imageFileSize = 0;

			imageResHandle = FindResourceW(NULL, resourceName.c_str(), resourceType.c_str());
			HRESULT hr = imageResHandle ? S_OK : E_FAIL;
			if (FAILED(hr)) {
				return hr;
			}
			imageResDataHandle = LoadResource(NULL, imageResHandle);
			hr = imageResDataHandle ? S_OK : E_FAIL;
			if (FAILED(hr)) {
				return hr;
			}
			pImageFile = LockResource(imageResDataHandle);
			hr = pImageFile ? S_OK : E_FAIL;
			if (SUCCEEDED(hr)) {
				imageFileSize = SizeofResource(NULL, imageResHandle);
				hr = imageFileSize ? S_OK : E_FAIL;
			}
			if (SUCCEEDED(hr)) {
				hr = m_pWICImageFactory->CreateStream(&pStream);
			}
			if (SUCCEEDED(hr)) {
				hr = pStream->InitializeFromMemory(
					reinterpret_cast<BYTE*>(pImageFile),
					imageFileSize);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pWICImageFactory->CreateDecoderFromStream(
					pStream,
					NULL,
					WICDecodeMetadataCacheOnLoad,
					&pDecoder);
			}
			if (SUCCEEDED(hr)) {
				hr = LoadBitmapFromDecoder(target, pDecoder, ppBitmap);
			}
			return hr;
		}
	private:
		CComPtr<ID2D1Factory>			m_pD2DFactory;
		CComPtr<IDWriteFactory>			m_pDWriteFactory;
		CComPtr<IWICImagingFactory>		m_pWICImageFactory;
		CComPtr<ID2D1HwndRenderTarget>	m_pRT;
		HWND							m_hWnd;
		D2D1_SIZE_U						m_size;

		long							m_cPushLayer;
	};
}
#endif //ZUI_ZDIRECT2DRENDER_HEADER