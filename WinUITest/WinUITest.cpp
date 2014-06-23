// WinUITest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinUITest.h"
#include "..\WinUI\ZHead.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
using namespace ZUI;
class HelloWindow :
	public ZUI::ZWinBase<HelloWindow>
{
public:
	
	HelloWindow() :
		ZWinBase<HelloWindow>()
	{}
	
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
	LPCTSTR lpName = L"zxcvd";
};
LONG ClickUp(ZControl* control, ZMouseState s)
{
	MessageBox(NULL, L"clickme", L"hi", 0);
	return 0;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HelloWindow *hellowindow = new HelloWindow();
	ZPaintManager *painter = new ZPaintManager();
	painter->Setup(L"direct2d");
	
	hellowindow->SetPaintManager(painter);
	hellowindow->Create(L"TestHellowindow", WS_OVERLAPPEDWINDOW);
	hellowindow->Show();
	ZLabel* label = new ZLabel();
	label->SetPosition(100, 100);
	label->SetText(L"hi");
	hellowindow->AddControl(label);
	ZButton* button = new ZButton();
	button->SetPosition(200, 200);
	button->SetText(L"click me");
	button->NotifyOnLButtonUp(ClickUp);
	hellowindow->AddControl(button);
	painter->MessageLoop();
	painter->Release();
	delete hellowindow;
	_CrtDumpMemoryLeaks();
	return 0;
}