#ifndef ZLIB_ZEVENT_HEADER
#define ZLIB_ZEVENT_HEADER
#include "ZBase.h"
#include "ZUIBase.h"
#include <iostream>
#include <vector>
namespace ZUI
{
	class ZControl;
	template<typename Func>
	class ZEvent :
		public ZObject
	{
	public:
		ZEvent()
		{}
		~ZEvent()
		{}
	public:
		void subscribe(Func func)
		{
			m_funcs.push_back(func);
		}
	protected:
		std::vector<Func> m_funcs;
	};
	enum
	{
		ZNoButtonPress = 0x0,
		ZLButtonPress = 0x1,
		ZRButtonPress = 0x2,
		ZSHIFTPress = 0x4,
		ZCTRLPress = 0x8,
		ZEXButtonPress = 0x16,
	};
	class ZMouseState :
		public ZObject
	{
	public:
		unsigned long buttonState;
		long x;
		long y;
		ZMouseState() :
			buttonState(0), x(0), y(0)
		{}
		ZMouseState(WPARAM wParam, LPARAM lParam) :
			buttonState(0)
		{
			x = lParam & 0xFFFF;
			y = (lParam >> 16) & 0xFFFF;
			if (wParam&MK_LBUTTON) {
				buttonState |= ZLButtonPress;
			}
			if (wParam&MK_RBUTTON) {
				buttonState |= ZRButtonPress;
			}
			if (wParam&MK_CONTROL) {
				buttonState |= ZCTRLPress;
			}
			if (wParam&MK_SHIFT) {
				buttonState |= ZSHIFTPress;
			}
		}
		BOOL LPress() const {
			return buttonState & ZLButtonPress;
		}
		BOOL RPress() const {
			return buttonState & ZRButtonPress;
		}
		BOOL CTRLPress() const {
			return buttonState & ZCTRLPress;
		}
		BOOL SHIFTPress() const {
			return buttonState & ZSHIFTPress;
		}
	};
	typedef LONG(*MouseFunc)(ZUI::ZControl*, ZUI::ZMouseState);
	class ZMouseEvent :
		public ZEvent<MouseFunc>
	{
	public:
		void Invoke(ZControl* control, ZMouseState s) {
			for each (auto func in m_funcs) {
				func(control, s);
			}
		}
	};
	typedef LONG(*ControlFunc)(ZUI::ZControl*, void*);
	class ZControlEvent :
		public ZEvent < ControlFunc >
	{
	public:
		void Invoke(ZControl* control, void* lpInfo) {
			for each (auto func in m_funcs) {
				func(control, lpInfo);
			}
		}
	};
} //namespace ZUI
#endif //ZLIB_ZEVENT_HEADER