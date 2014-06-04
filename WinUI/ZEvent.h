#ifndef ZLIB_ZEVENT_HEADER
#define ZLIB_ZEVENT_HEADER
#include "ZBase.h"
#include <iostream>
#include <vector>
namespace ZUI
{
	class ZEvent :
		public ZObject
	{
	public:
		typedef void(*zeveFunc)(void);
	public:
		ZEvent()
		{}
		~ZEvent()
		{}
	public:
		void subscribe(zeveFunc func)
		{
			m_funcs.push_back(func);
		}
		virtual void Invoke()
		{
			for each (auto func in m_funcs) {
				func();
			}
		}
	private:
		std::vector<zeveFunc> m_funcs;
	};
} //namespace ZUI
#endif //ZLIB_ZEVENT_HEADER