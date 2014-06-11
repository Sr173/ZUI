#ifndef ZUI_ZRENDER_HEADER
#define ZUI_ZRENDER_HEADER
#include "ZBase.h"
#include <Windows.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <atlbase.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
namespace ZUI
{
	class ZRender :
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
		virtual ~ZRender()
		{}
	public:
		virtual bool PaintText(RECT rc, ZStringW text,
			int fontSize, ZColor fontColor,
			TextAlignMode alignModeX, TextAlignMode alignModeY) = 0;
		virtual bool FillRectangle(RECT rc, ZColor color) = 0;
		virtual bool DrawRectangle(RECT rc, ZColor color) = 0;
		virtual bool DrawLine(ZColor color, POINT pt1, POINT pt2) = 0;
		virtual bool Resize(RECT rc) = 0;
		virtual bool Clear(ZColor color) = 0;
		virtual void Release() = 0;
	};
}
#endif //ZUI_ZRENDER_HEADER