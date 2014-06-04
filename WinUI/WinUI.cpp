// WinUI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinUI.h"
#include "ZHead.h"
#include "ZCommonWnd.h"
class HelloWindow :
	public ZUI::ZWinBase<HelloWindow>
{
public:
	~HelloWindow()
	{}
public:
	LPCTSTR ClassName() const
	{
		return lpName;
	}
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;
		switch (uMsg) {
		case WM_PAINT:
			PAINTSTRUCT ps;
			hdc = ::BeginPaint(hWnd, &ps);
			RECT rc;
			GetWindowRect(hWnd, &rc);
			DrawText(hdc, _T("hihi,helloWindow"), 12, &rc, 0);
			::EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}
private:
	LPCWSTR lpName = L"adsfdsffff";
};
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ZUI::ZCommonWnd::CmnMsgWnd* hellowindow = new ZUI::ZCommonWnd::CmnMsgWnd();
	hellowindow->Create(L"hi", WS_VISIBLE | WS_OVERLAPPEDWINDOW, WS_EX_STATICEDGE | WS_EX_APPWINDOW,
		0, 0, 300, 500);
	hellowindow->SetText(L"Hi, World!");
	hellowindow->Show();
	ZUI::ZCommonWnd::CmnMsgWnd::MessageLoop();
	return 0;
}

