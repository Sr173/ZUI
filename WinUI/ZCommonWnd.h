#ifndef ZUI_ZCOMMONWND_HEADER
#define ZUI_ZCOMMONWND_HEADER
#include "ZHead.h"
namespace ZUI
{
	namespace ZCommonWnd
	{
		class CmnMsgWnd :
			public ZWinBase<CmnMsgWnd>
		{
		public:
			CmnMsgWnd() :
				m_lpText(NULL), m_fontSize(24),
				m_backColor(255, 255, 255),
				m_fontColor(0, 0, 0)
			{}
			~CmnMsgWnd()
			{}
		public:
			BOOL Create(LPCWSTR lpWindowName, int nWidth, int nHeight, LPCWSTR text = NULL, HWND parent = NULL);
			CmnMsgWnd& SetText(LPCWSTR text);
			CmnMsgWnd& SetTextColor(BYTE r, BYTE g, BYTE b, BYTE a = 255);
			CmnMsgWnd& SetBackColor(BYTE r, BYTE g, BYTE b, BYTE a = 255);
			void Close() {
				PostQuitMessage(0);
			}
		public:
			virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			virtual LPCTSTR ClassName() const;
		private:
			void OnPaint(HDC hdc);
		private:
			LPWSTR		m_lpText;  //文字
			ZColor		m_fontColor; //文字颜色
			ZColor		m_backColor; //背景颜色
			int			m_fontSize; //字体大小
		};
	}
}
#endif //ZUI_ZCOMMONWND_HEADER