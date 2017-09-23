#pragma once
class AboutBox
{
public:
	AboutBox();
	~AboutBox();
	void Create(HINSTANCE hInstance, HWND hParent);

private:
	// Dialog Proc
	static INT_PTR CALLBACK AboutBox::StaticAboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK AboutBox::AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// handlers
	INT_PTR AboutBox::OnInitDialog(HWND hDlg);

	// member variables
	HWND _hWnd;
};

