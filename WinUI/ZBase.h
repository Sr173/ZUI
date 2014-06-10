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
		ZNullable() :
			m_bNull(true)
		{}
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
			ZNullable(true), m_buffer(nullptr), m_length(0)
		{}
		ZStringA(const char* str) :
			ZNullable(false)
		{
			m_length = strlen(str);
			m_buffer = new char[m_length + 1];
			strcpy_s(m_buffer, m_length + 1, str);
		}
		~ZStringA()
		{
			Clear();
		}
		ZStringA(const ZStringA& str) :
			ZNullable(true)
		{
			*this = str;
		}
		ZStringA& operator=(const ZStringA& str)
		{
			SetNull(str.IsNull());
			if (!IsNull()) {
				m_length = str.Length();
				m_buffer = new char[m_length + 1];
				strcpy_s(m_buffer, m_length + 1, str.m_buffer);
			}
			return *this;
		}
		bool operator==(const ZStringA& str) const
		{
			if (IsNull() && str.IsNull()) {
				return true;
			}
			if (IsNull() || str.IsNull()) {
				return false;
			}
			return (!strcmp(m_buffer, str.m_buffer));
		}
		bool operator!=(const ZStringA& str) const
		{
			return !(*this == str);
		}
		ZStringA& operator+=(const ZStringA& str)
		{
			Append(str);
			return *this;
		}
		ZStringA operator+(const ZStringA& str)
		{
			ZStringA tmpStr = *this;
			tmpStr += str;
			return tmpStr;
		}
	public:
		void Clear()
		{
			if (m_buffer != nullptr) {
				delete[] m_buffer;
				m_buffer = nullptr;
			}
			SetNull(true);
			m_length = 0;
		}
		unsigned long Length() const {
			return m_length;
		}
		const char* c_str()
		{
			return m_buffer;
		}
		unsigned long Append(const ZStringA& str)
		{
			return Append(str.m_buffer);
		}
		unsigned long Append(const char* str)
		{
			int length = m_length + strlen(str);
			char* newBuffer = new char[length + 1];
			newBuffer[0] = 0;
			if (!IsNull()) {
				strcpy_s(newBuffer, length + 1, m_buffer);
			}
			strcat_s(newBuffer, length + 1, str);
			Clear();
			m_buffer = newBuffer;
			m_length = length;
			SetNull(false);
			return m_length;
		}
		unsigned long Append(char c)
		{
			char tmpStr[] = { c, 0 };
			return Append(tmpStr);
		}
		ZStringA ToLower() const
		{
			ZStringA lowerStr(*this);
			unsigned long len = Length();
			for (unsigned long i = 0; i < len; ++i)
			{
				if (m_buffer[i] <= 'Z'
					&& m_buffer[i] >= 'A') {
					m_buffer[i] -= ('A' - 'a');
				}
			}
			return lowerStr;
		}
	private:
		char*		m_buffer;
		unsigned long	m_length;
	};
	class ZStringW :
		public ZNullable
	{
	public:
		ZStringW() :
			ZNullable(true), m_buffer(nullptr), m_length(0)
		{}
		ZStringW(const wchar_t* str) :
			ZNullable(false)
		{
			m_length = wcslen(str);
			m_buffer = new wchar_t[m_length + 1];
			wcscpy_s(m_buffer, m_length + 1, str);
		}
		~ZStringW()
		{
			Clear();
		}
		ZStringW(const ZStringW& str) :
			ZNullable(true)
		{
			*this = str;
		}
		ZStringW& operator=(const ZStringW& str)
		{
			SetNull(str.IsNull());
			if (!IsNull()) {
				m_length = str.Length();
				m_buffer = new wchar_t[m_length + 1];
				wcscpy_s(m_buffer, m_length + 1, str.m_buffer);
			}
			return *this;
		}
		bool operator==(const ZStringW& str) const
		{
			if (IsNull() && str.IsNull()) {
				return true;
			}
			if (IsNull() || str.IsNull()) {
				return false;
			}
			return (!wcscmp(m_buffer, str.m_buffer));
		}
		bool operator!=(const ZStringW& str) const
		{
			return !(*this == str);
		}
		ZStringW& operator+=(const ZStringW& str)
		{
			Append(str);
			return *this;
		}
		ZStringW operator+(const ZStringW& str)
		{
			ZStringW tmpStr = *this;
			tmpStr += str;
			return tmpStr;
		}
	public:
		void Clear()
		{
			if (m_buffer != nullptr) {
				delete[] m_buffer;
				m_buffer = nullptr;
			}
			SetNull(true);
			m_length = 0;
		}
		unsigned long Length() const {
			return m_length;
		}
		const wchar_t* c_str()
		{
			return m_buffer;
		}
		unsigned long Append(const ZStringW& str)
		{
			return Append(str.m_buffer);
		}
		unsigned long Append(const wchar_t* str)
		{
			int length = m_length + wcslen(str);
			wchar_t* newBuffer = new wchar_t[length + 1];
			newBuffer[0] = 0;
			if (!IsNull()) {
				wcscpy_s(newBuffer, length + 1, m_buffer);
			}
			wcscat_s(newBuffer, length + 1, str);
			Clear();
			m_buffer = newBuffer;
			m_length = length;
			SetNull(false);
			return m_length;
		}
		unsigned long Append(wchar_t c)
		{
			wchar_t tmpStr[] = { c , 0 };
			return Append(tmpStr);
		}
		ZStringW ToLower() const
		{
			ZStringW lowerStr(*this);
			unsigned long len = Length();
			for (unsigned long i = 0; i < len; ++i)
			{
				if (m_buffer[i] <= L'Z'
					&& m_buffer[i] >= L'A') {
					m_buffer[i] -= (L'A' - L'a');
				}
			}
			return lowerStr;
		}
	private:
		wchar_t*		m_buffer;
		unsigned long	m_length;
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