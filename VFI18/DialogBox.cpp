#include "stdafx.h"
#include "DialogBox.h"

MyDialogBox::MyDialogBox()
{
}

MyDialogBox::~MyDialogBox()
{
}

INT_PTR CALLBACK MyDialogBox::StaticAboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG)
	{
		SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam);
	}

	MyDialogBox* pThis = (MyDialogBox*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (pThis != NULL)
	{
		return pThis->AboutDialogProc(hDlg, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
}

void  MyDialogBox::Create(HINSTANCE hInstance, HWND hParent)
{
	DialogBoxParamW(hInstance, MAKEINTRESOURCE(DIALOG_ABOUTBOX), hParent, &MyDialogBox::StaticAboutDialogProc, (LPARAM)this);
}

INT_PTR MyDialogBox::OnInitDialog(HWND hDlg)
{
	_hWnd = hDlg;
	return (INT_PTR)TRUE;
}

INT_PTR CALLBACK MyDialogBox::AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(_hWnd, LOWORD(wParam));
			}
			return (INT_PTR)TRUE;
		}

		case WM_INITDIALOG:
		{
			return OnInitDialog(hDlg);
		}

		default:
		{
			return DefWindowProc(hDlg, message, wParam, lParam);
		}
	}

	return (INT_PTR)FALSE;
}