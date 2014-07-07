#ifndef ZUI_ZRENDER_HEADER
#define ZUI_ZRENDER_HEADER
#include "ZBase.h"

namespace ZUI
{
	enum ZRenderResult
	{
		ZRENDER_OK = 1,
		ZRENDER_FAIL = -1,
		ZRENDER_RECTREATE_TARGET = -100,
		ZRENDER_BAD_PARAM = -101,
	};
	class ZBrush :
		public ZObject,
		public NoCopyable
	{
	public:
		virtual ~ZBrush()
		{}
	public:
		virtual void Release() = 0;
	};
	class ZSolidBrush :
		public ZBrush
	{
	public:
		virtual ~ZSolidBrush()
		{}
	public:
		virtual ZColor GetColor() const = 0;
		virtual void SetColor(ZColor color) = 0;

	};
	class ZFontFormat :
		public ZObject,
		public NoCopyable
	{
	public:
		enum TextAlignMode
		{
			TAMNormal = 0,
			TAMNear = 1,
			TAMMiddle = 2,
			TAMFar = 3,
		};
	public:
		virtual ~ZFontFormat()
		{}
		virtual void Release() = 0;
	public:
		virtual void SetTextAlignment(TextAlignMode mode) = 0;
		virtual void SetParagraphAlignment(TextAlignMode mode) = 0;
	};
	class ZRender :
		public ZObject,
		public NoCopyable
	{
	public:
		virtual ~ZRender()
		{}
	public:
		virtual ZRenderResult	BeginRender() = 0;
		virtual ZRenderResult	EndRender() = 0;
	public:
		virtual ZRenderResult	CreateSolidBrush(ZColor color, ZSolidBrush** ppBrush) = 0;
		virtual ZRenderResult	CreateFontFormat(ZStringW fontFamily, long fontSize, ZFontFormat** ppFontFormat) = 0;
	public:
		virtual ZRenderResult	DrawRectangle(ZRect rc, ZBrush* pBrush) = 0;
		virtual ZRenderResult	DrawLine(ZPoint pt1, ZPoint pt2, ZBrush* pBrush) = 0;
		virtual ZRenderResult	FillRectangle(ZRect rc, ZBrush* pBrush) = 0;
		virtual ZRenderResult	DrawEllipse(ZPoint pt, long rx, long ry, ZBrush* pBrush) = 0;
		virtual ZRenderResult	PaintText(ZStringW text, ZRect rc, ZFontFormat* format, ZBrush* pBrush) = 0;
		virtual ZRenderResult	ReSize(ZSize size) = 0;
		virtual ZRenderResult	PushLayer(const ZRect& rc) = 0;
		virtual ZRenderResult	PopLayer() = 0;
		virtual ZRenderResult	Clear(ZColor color) = 0;
	public:
		virtual void			Release() = 0;
	};
	ZRender* CreateDirect2DRender(HWND hWnd, ZSize size);
}
#endif //ZUI_ZRENDER_HEADER