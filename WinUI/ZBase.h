#ifndef ZUI_ZBASE_HEADER
#define ZUI_ZBASE_HEADER
#include <iostream>
#include <windows.h>
namespace ZUI
{
	class ZObject
	{
	public:
		virtual ~ZObject()
		{}
	};
	class NoCopyable :
		public ZObject
	{
	public:
		NoCopyable()
		{}
		virtual ~NoCopyable()
		{}
	private:
		NoCopyable(const NoCopyable&)
		{}
		void operator=(const NoCopyable&)
		{}
	};
	class ZNullable :
		public ZObject
	{
	public:
		ZNullable(bool bNull) :
			m_bNull(bNull)
		{}
		virtual ~ZNullable()
		{}
	public:
		bool IsNull() const {
			return m_bNull;
		}
		void SetNull(bool b) {
			m_bNull = b;
		}
	private:
		bool m_bNull;
	};

	class ZStringA :
		public ZNullable
	{
	public:
		ZStringA() :
			ZNullable(true)
		{}
		ZStringA(const char* str) :
			ZNullable(false), m_str(str)
		{}
		~ZStringA()
		{}
		ZStringA(const ZStringA& str) :
			ZNullable(str.IsNull())
		{
			*this = str;
		}
		ZStringA& operator=(const ZStringA& str)
		{
			SetNull(str.IsNull());
			m_str = str.m_str;
		}
		bool operator==(const ZStringA& str)
		{
			if (IsNull() && str.IsNull()) {
				return true;
			}
			if (IsNull() || str.IsNull()) {
				return false;
			}
			return m_str == str.m_str;
		}
	public:
		operator const char*() {
			return m_str.c_str();
		}
	private:
		std::string m_str;
	};
	class ZStringW :
		public ZNullable
	{
	public:
		ZStringW() :
			ZNullable(true)
		{}
		ZStringW(const wchar_t* str) :
			ZNullable(false), m_str(str)
		{}
		~ZStringW()
		{}
		ZStringW(const ZStringW& str) :
			ZNullable(str.IsNull())
		{
			*this = str;
		}
		ZStringW& operator=(const ZStringW& str)
		{
			SetNull(str.IsNull());
			m_str = str.m_str;
			return *this;
		}
		bool operator==(const ZStringW& str)
		{
			if (IsNull() && str.IsNull()) {
				return true;
			}
			if (IsNull() || str.IsNull()) {
				return false;
			}
			return m_str == str.m_str;
		}
	public:
		operator const wchar_t*()
		{
			return m_str.c_str();
		}
	private:
		std::wstring m_str;
	};
#ifndef UNICODE
	typedef ZStringA ZString;
#else
	typedef ZStringW ZString;
#endif // UNICODE
	struct ZColor {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;
		ZColor(BYTE _r, BYTE _g, BYTE _b, BYTE _a = 255)
		{
			r = _r; g = _g; b = _b; a = _a;
		}
	};
	inline bool PointInRect(long x, long y, const RECT& rc)
	{
		return (rc.left <= x&&rc.right >= x&&
			rc.bottom >= y&&rc.top <= y) ? true : false;
	}
	inline bool PointInRect(const POINT& pt, const RECT& rc)
	{
		return PointInRect(pt.x, pt.y, rc);
	}
	bool IsRectCross(const RECT& rc1, const RECT& rc2);
}
#endif //ZUI_ZBASE_HEADER