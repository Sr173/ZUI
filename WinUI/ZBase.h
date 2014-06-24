#ifndef ZUI_ZBASE_HEADER
#define ZUI_ZBASE_HEADER
#include <iostream>
#include <list>
#include <windows.h>
namespace ZUI
{
	class ZObject
	{
	public:
		virtual ~ZObject()
		{}
		virtual const wchar_t* GetType() const {
			return L"object";
		}
	};
	class NoCopyable
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
	template<typename T>
	class ZAutoReleasePtr :
		public ZObject
	{
	public:
		ZAutoReleasePtr(T* p) :
			m_p(p), m_count(1)
		{}
		~ZAutoReleasePtr()
		{
			Release();
		}
	public:
		ZAutoReleasePtr(const ZAutoReleasePtr<T>& p)
		{}
		ZAutoReleasePtr& operator=(
			const ZAutoReleasePtr<T>& p)
		{
			return *this;
		}
	public:
		unsigned long AddRef()
		{
			return 0;
		}
		void Release()
		{
		}
	private:
		unsigned long*	m_count;
		T*				m_p;
	};
	class ZStringA :
		public ZNullable
	{
	public:
		ZStringA() :
			ZNullable(true), m_buffer(nullptr), m_length(0)
		{}
		ZStringA(const char* str) :
			ZNullable(false), m_buffer(nullptr), m_length(0)
		{
			if (str == nullptr) {
				SetNull(true);
			}
			else {
				m_length = strlen(str);
				m_buffer = new char[m_length + 1];
				strcpy_s(m_buffer, m_length + 1, str);
			}
		}
		~ZStringA()
		{
			Clear();
		}

		ZStringA(const ZStringA& str) :
			ZNullable(true), m_buffer(nullptr), m_length(0)
		{
			*this = str;
		}
		ZStringA& operator=(const ZStringA& str)
		{
			Clear();
			if (!str.IsNull()) {
				SetNull(false);
				m_length = str.Length();
				m_buffer = new char[m_length + 1];
				strcpy_s(m_buffer, m_length + 1, str.m_buffer);
			}
			else {
				SetNull(true);
			}
			return *this;
		}
		virtual const wchar_t* GetType() const {
			return L"stringA";
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
					lowerStr.m_buffer[i] -= ('A' - 'a');
				}
			}
			return lowerStr;
		}
		ZStringA SubString(unsigned long start, unsigned long end) const
		{
			ZStringA str;
			if (start >= Length() || end > Length()) {
				return str;
			}
			char* buffer = new char[end - start + 2];
			for (unsigned long i = start; i < end; ++i) {
				buffer[i - start] = m_buffer[i];
			}
			buffer[end - start] = 0;
			str = buffer;
			delete[] buffer;
			return str;
		}
		long Find(ZStringA subString, long start = 0, long end = -1)
		{
			long subLength = subString.Length();
			if (end == -1) end = Length();
			for (long i = start; i <= end - subLength; ++i)
			{
				bool bFlag = true;
				for (long j = 0; j < subLength; ++j)
				{
					if (subString.m_buffer[j] != m_buffer[i + j]) {
						bFlag = false;
						j = subLength;
					}
				}
				if (bFlag) {
					return i;
				}
			}
			return -1;
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
			ZNullable(false), m_buffer(nullptr), m_length(0)
		{
			if (str == nullptr) {
				SetNull(true);
			}
			else {
				m_length = wcslen(str);
				m_buffer = new wchar_t[m_length + 1];
				wcscpy_s(m_buffer, m_length + 1, str);
			}
		}
		~ZStringW()
		{
			Clear();
		}
		ZStringW(const ZStringW& str) :
			ZNullable(true), m_buffer(nullptr), m_length(0)
		{
			*this = str;
		}
		ZStringW& operator=(const ZStringW& str)
		{
			Clear();
			if (!str.IsNull()) {
				SetNull(false);
				m_length = str.Length();
				m_buffer = new wchar_t[m_length + 1];
				wcscpy_s(m_buffer, m_length + 1, str.m_buffer);
			}
			else {
				SetNull(true);
			}
			return *this;
		}
		virtual const wchar_t* GetType() const {
			return L"stringW";
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
			tmpStr.Append(str);
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
			wchar_t tmpStr[] = { c, 0 };
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
					lowerStr.m_buffer[i] -= (L'A' - L'a');
				}
			}
			return lowerStr;
		}
		ZStringW SubString(unsigned long start, unsigned long end) const
		{
			ZStringW str;
			if (start >= Length() || end > Length()) {
				return str;
			}
			wchar_t* buffer = new wchar_t[end - start + 2];
			for (unsigned long i = start; i < end; ++i) {
				buffer[i - start] = m_buffer[i];
			}
			buffer[end - start] = 0;
			str = buffer;
			delete[] buffer;
			return str;
		}
		long Find(ZStringW subString, long start = 0, long end = -1)
		{
			long subLength = subString.Length();
			if (end == -1) end = Length();
			for (long i = start; i <= end - subLength; ++i)
			{
				bool bFlag = true;
				for (long j = 0; j < subLength; ++j)
				{
					if (subString.m_buffer[j] != m_buffer[i + j]) {
						bFlag = false;
						j = subLength;
					}
				}
				if (bFlag) {
					return i;
				}
			}
			return -1;
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
	struct ZRect {
		long left;
		long top;
		long right;
		long bottom;
	};
	struct ZSize {
		long width;
		long height;
	};
	struct ZPoint {
		long x;
		long y;
	};
	inline bool PointInRect(long x, long y, const ZRect& rc)
	{
		return (rc.left <= x&&rc.right >= x&&
			rc.bottom >= y&&rc.top <= y) ? true : false;
	}
	inline bool PointInRect(const POINT& pt, const ZRect& rc)
	{
		return PointInRect(pt.x, pt.y, rc);
	}
	bool IsRectCross(const ZRect& rc1, const ZRect& rc2);
	
}
#endif //ZUI_ZBASE_HEADER