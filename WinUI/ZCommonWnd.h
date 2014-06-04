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
				m_lpText(NULL)
			{}
			~CmnMsgWnd()
			{}
		public:
			void SetText(LPCWSTR text);
		public:
			virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			virtual LPCTSTR ClassName() const;
		private:
			void OnPaint(HDC hdc);
		private:
			LPWSTR m_lpText;
		};
	}
}
#endif //ZUI_ZCOMMONWND_HEADER