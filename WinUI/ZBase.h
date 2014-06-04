#ifndef ZUI_ZBASE_HEADER
#define ZUI_ZBASE_HEADER
#include <iostream>
namespace ZUI
{
#ifndef UNICODE
	typedef std::string ZString;
#else
	typedef std::wstring ZString;
#endif // UNICODE
	class ZObject
	{
	public:
		virtual ~ZObject()
		{}
	};
}
#endif //ZUI_ZBASE_HEADER