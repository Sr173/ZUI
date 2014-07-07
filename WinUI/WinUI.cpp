// WinUI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinUI.h"
#include "ZHead.h"
#include "ZCommonWnd.h"
#include <windowsx.h>
#include <windows.h>

//检测内存泄漏
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
	LPCWSTR lpName = L"adsfdsffff";
};
LONG BeClick(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = dynamic_cast<ZUI::ZLabel*>(con);
	ZUI::ZString str = label->GetText();
	::MessageBox(NULL, str.c_str(), _T("Label Click"), 0);
	return 0;
}
LONG ClearText(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	HelloWindow* wnd = reinterpret_cast<HelloWindow*>(
		con->GetParentClass());
	ZUI::ZTextBox* textBox = dynamic_cast<ZUI::ZTextBox*>(
		wnd->FindControl(_T("textBox")));
	textBox->SetText(L"");
	return 0;
}
LONG RClick(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	::MessageBox(NULL, _T("Right Click"), _T("Label"), 0);
	return 0;
}
LONG MoveIn(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = dynamic_cast<ZUI::ZLabel*>(con);
	label->SetBackColor(0x66, 0xcc, 0xff, 127);
	label->Invalidate();
	return 0;
}
LONG MoveOut(ZUI::ZControl* con, ZUI::ZMouseState s)
{
	ZUI::ZLabel* label = dynamic_cast<ZUI::ZLabel*>(con);
	label->SetBackColor(0x66, 0xcc, 0xff, 255);
	label->Invalidate();
	return 0;
}
LONG GetFocus(ZUI::ZControl* con, void* pInfo)
{
	::MessageBox(NULL, _T("GetFocus"), _T("hi"), 0);
	return 0;
}
LONG LostFocus(ZUI::ZControl* con, void* pInfo)
{
	::MessageBox(NULL, _T("LostFocus"), _T("hi"), 0);
	return 0;
}
LONG CheckedChange(ZUI::ZControl* con, void* pInfo)
{
	ZUI::ZCheckBox* pThis = dynamic_cast<ZUI::ZCheckBox*>(con);
	HelloWindow* wnd = reinterpret_cast<HelloWindow*>(
		pThis->GetParentClass());
	ZUI::ZLabel* label = dynamic_cast<ZUI::ZLabel*>(
		wnd->FindControl(_T("checkLabel")));
	ZUI::ZButton* button = dynamic_cast<ZUI::ZButton*>(
		wnd->FindControl(_T("button1")));
	if (pThis->IsChecked()) {
		label->SetText(L"selected");
		button->SetVisible(true);
	}
	else {
		label->SetText(L"unselected");
		button->SetVisible(false);
	}
	return 0;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ZUI::ZPaintManager *paintManager = new ZUI::ZPaintManager();
	paintManager->Setup(_T("direct2d"));
	//paintManager->Setup(_T("gdiplus"));
	HelloWindow* hellowindow = new HelloWindow();
	hellowindow->SetPaintManager(paintManager);

	ZUI::ZLabel* label = new ZUI::ZLabel();
	label->SetPosition(100, 100);
	label->SetWidth(200);
	label->SetId(_T("checkLabel"));
	label->SetText(L"Control Test");
	label->SetBackColor(0x66, 0xcc, 0xff, 255);
	//label->SetBorderColor(0xff, 0, 0);
	label->NotifyOnLButtonUp(BeClick);
	label->NotifyOnRButtonUp(RClick);
	label->NotifyOnMouseMoveIn(MoveIn);
	label->NotifyOnMouseMoveOut(MoveOut);


	
	

	ZUI::ZButton* button1 = new ZUI::ZButton();
	button1->SetPosition(50, 200);
	//button1->SetWidth(100);
	button1->SetHoverColor(0xff, 0x00, 0xff, 127);
	button1->SetText(L"测试按钮");
	button1->SetId(_T("button1"));
	button1->SetBackColor(0x66, 0xcc, 0xff, 255);
	button1->NotifyOnLButtonUp(BeClick);
	button1->NotifyOnRButtonUp(RClick);
	ZUI::ZLabel* label2 = new ZUI::ZLabel();
	hellowindow->AddControl(label2);
	hellowindow->RemoveControl(label2);
	//hellowindow->AddControl(label1);

	/*
	ZUI::ZPaintManager* paintManager1 = new ZUI::ZPaintManager();
	paintManager1->Setup(_T("direct2d"));
	
	ZUI::ZCommonWnd::CmnMsgWnd* msgwindow = new ZUI::ZCommonWnd::CmnMsgWnd();
	msgwindow->SetPaintManager(paintManager1);

	ZUI::ZButton* button2 = new ZUI::ZButton();
	button2->SetPosition(50, 50);
	//button1->SetWidth(100);
	button2->SetHoverColor(0xff, 0x00, 0xff, 127);
	button2->SetText(L"测试按钮");
	button2->SetBackColor(0x66, 0xcc, 0xff, 255);
	button2->NotifyOnLButtonUp(BeClick);
	button2->NotifyOnRButtonUp(RClick);

	ZUI::ZLabel* label1 = new ZUI::ZLabel();
	label1->SetPosition(50, 100);
	label1->SetText(L"Son Test");
	label1->SetBackColor(0x66, 0xcc, 0xff, 255);
	label1->NotifyOnLButtonUp(BeClick);
	label1->NotifyOnRButtonUp(RClick);
	label1->NotifyOnMouseMoveIn(MoveIn);
	label1->NotifyOnMouseMoveOut(MoveOut);

	msgwindow->AddControl(label1);
	msgwindow->AddControl(button2);
	msgwindow->Create(L"hihi", 300, 200, NULL, hellowindow->GetHWND());
	msgwindow->SetText(L"Hi, World!");
	msgwindow->SetTextColor(0x66, 0xcc, 0xff);
	msgwindow->SetBackColor(0, 0, 0);
	msgwindow->Show();
	*/
	ZUI::ZTextBox* textbox = new ZUI::ZTextBox();
	textbox->SetPosition(50, 250);
	textbox->SetHeight(50);
	textbox->SetTextSize(24);
	textbox->SetId(_T("textBox"));
	
	ZUI::ZButton* lbutton1 = new ZUI::ZButton();
	lbutton1->SetPosition(150, 150);
	lbutton1->SetText(_T("layout button"));
	lbutton1->NotifyOnLButtonUp(BeClick);
	ZUI::ZLayout* layout = new ZUI::ZLayout();
	ZUI::ZRect layoutRc = { 0, 0, 300, 500 };
	layout->SetRect(layoutRc);
	layout->AddControl(lbutton1);
	hellowindow->AddControl(layout);
	
	ZUI::ZCheckBox* checkBox1 = new ZUI::ZCheckBox();
	checkBox1->SetText(L"你好");
	checkBox1->SetPosition(200, 200);
	checkBox1->SetBorderColor(255, 255, 255);
	checkBox1->NotifyOnCheckedChanged(CheckedChange);
	layout->AddControl(checkBox1);
	ZUI::ZButton* clearButton = new ZUI::ZButton();
	clearButton->SetText(L"清除文本框");
	clearButton->SetPosition(50, 300);
	clearButton->SetWidth(80);
	clearButton->NotifyOnLButtonUp(ClearText);
	/*
	textbox->NotifyOnGetFocus(GetFocus);
	textbox->NotifyOnLostFocus(LostFocus);
	*/
	hellowindow->AddControl(clearButton);
	hellowindow->AddControl(textbox);
	hellowindow->AddControl(button1);
	hellowindow->AddControl(label);
	hellowindow->Create(L"hellowindow", WS_OVERLAPPEDWINDOW);
	hellowindow->Show();

	ZUI::ZPaintManager::MessageLoop();
	paintManager->Release();
	delete hellowindow;
	//delete msgwindow;
	_CrtDumpMemoryLeaks();
	return 0;
}

