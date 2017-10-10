#include "stdafx.h"
#include "AboutBox.h"

AboutBox::AboutBox()
{
}

AboutBox::~AboutBox()
{
}

INT_PTR CALLBACK AboutBox::StaticAboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG)
	{
		SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam);
	}

	AboutBox* pThis = (AboutBox*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (pThis != NULL)
	{
		return pThis->AboutDialogProc(hDlg, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
}

void AboutBox::Create(HINSTANCE hInstance, HWND hParent )
{
	DialogBoxParamW(hInstance, MAKEINTRESOURCE(DIALOG_ABOUTBOX), hParent, &AboutBox::StaticAboutDialogProc, (LPARAM)this);
}

INT_PTR AboutBox::OnInitDialog(HWND hDlg)
{
	_hWnd = hDlg;
	return (INT_PTR)TRUE;
}

INT_PTR CALLBACK AboutBox::AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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