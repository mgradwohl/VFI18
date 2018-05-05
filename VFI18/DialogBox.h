#pragma once

#include "Window.h"

class MyDialogBox : public Window
{
public:
	MyDialogBox();
	~MyDialogBox();
	void Create(HINSTANCE hInstance, HWND hParent);

private:
	// Dialog Proc
	static INT_PTR CALLBACK MyDialogBox::StaticAboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK MyDialogBox::AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// handlers
	INT_PTR MyDialogBox::OnInitDialog(HWND hDlg);

	// member variables
	HWND _hWnd;
};

