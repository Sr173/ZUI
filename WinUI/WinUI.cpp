// WinUI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinUI.h"
#include "ZHead.h"
#include "ZCommonWnd.h"
#include <windowsx.h>
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
			::GetWindowRect(hWnd, &rc);
			//DrawText(hdc, _T("hihi,helloWindow"), 12, &rc, 0);

			::EndPaint(hWnd, &ps);
			break;
		case WM_SIZE:
			Invalidate();
			break;
		case WM_MOVE:
			Invalidate();
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
LONG BeClick(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = reinterpret_cast<ZUI::ZLabel*>(con);
	ZUI::ZString str = label->GetText();
	::MessageBox(NULL, str, _T("Label Click"), 0);
	return 0;
}
LONG RClick(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	::MessageBox(NULL, _T("Right Click"), _T("Label"), 0);
	return 0;
}
LONG MoveIn(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = reinterpret_cast<ZUI::ZLabel*>(con);
	label->SetBackColor(255, 0, 0, 127);
	label->Invalidate();
	return 0;
}
LONG MoveOut(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = reinterpret_cast<ZUI::ZLabel*>(con);
	label->SetBackColor(0x66, 0xcc, 0xff, 127);
	label->Invalidate();
	return 0;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ZUI::ZGdiplusManager *paintManager = new ZUI::ZGdiplusManager();
	HelloWindow* hellowindow = new HelloWindow();
	hellowindow->SetPaintManager(paintManager);
	ZUI::ZLabel* label = new ZUI::ZLabel();
	label->SetPosition(100, 100);
	label->SetWidth(200);
	label->SetText(L"Control Test");
	label->SetBackColor(0x66, 0xcc, 0xff, 127);
	label->NotifyOnLButtonUp(BeClick);
	label->NotifyOnRButtonUp(RClick);
	label->NotifyOnMouseMoveIn(MoveIn);
	label->NotifyOnMouseMoveOut(MoveOut);

	hellowindow->AddControl(label);
	hellowindow->Create(L"hellowindow", WS_OVERLAPPEDWINDOW);
	hellowindow->Show();
	
	ZUI::ZLabel* label1 = new ZUI::ZLabel();
	label1->SetPosition(50, 100);
	label1->SetText(L"Son Test");	
	label1->SetBackColor(0x66, 0xcc, 0xff, 127);
	label1->NotifyOnLButtonUp(BeClick);
	label1->NotifyOnRButtonUp(RClick);
	label1->NotifyOnMouseMoveIn(MoveIn);
	label1->NotifyOnMouseMoveOut(MoveOut);
	hellowindow->AddControl(label1);
	ZUI::ZCommonWnd::CmnMsgWnd* msgwindow = new ZUI::ZCommonWnd::CmnMsgWnd();
	msgwindow->SetPaintManager(paintManager);
	//msgwindow->AddControl(label1);
	msgwindow->Create(L"hihi", 300, 200, NULL/*, hellowindow->GetHWND()*/);
	msgwindow->SetText(L"Hi, World!");
	msgwindow->SetTextColor(0x66, 0xcc, 0xff);
	msgwindow->SetBackColor(0, 0, 0);
	msgwindow->Show();
	
	ZUI::ZGdiplusManager::MessageLoop();
	paintManager->Release();
	return 0;
}

