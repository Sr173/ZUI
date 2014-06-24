#include "Test.h"
#include <Windows.h>
#include <process.h>
using namespace ZUI;
class HelloWindow :
	public ZUI::ZWinBase<HelloWindow>
{
public:
	virtual ~HelloWindow()
	{}
public:
	LPCTSTR ClassName() const
	{
		return lpName;
	}
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
private:
	LPCWSTR lpName = L"zxcvd";
};
unsigned int __stdcall testThread(void* lpParam)
{
	HelloWindow hellowindow;
	ZPaintManager *painter = new ZPaintManager();
	painter->Setup(L"direct2d");
	hellowindow.SetPaintManager(painter);
	hellowindow.Create(L"TestHellowindow", WS_OVERLAPPEDWINDOW);
	hellowindow.Show();
	painter->MessageLoop();
	painter->Release();
	return 0;
}
void HelloWndTest()
{
	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL, 0, testThread, NULL, 0, NULL);
}
void ZWndTest()
{

}
