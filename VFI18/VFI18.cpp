// VFI18.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VFI18.h"
#include "window.h"

// Global Variables:
HINSTANCE g_hInstance;
wstring g_szTitle;
wstring g_szWindowClass;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OnCommand(HWND hWnd, int id, HWND hwndCtrl, UINT codeNotify);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Window appWindow;
	if (!appWindow.SetWindowClassAttributes(IDI_VFI18, IDI_SMALL, IDC_VFI18, IDC_VFI18, IDS_APP_TITLE, IDC_VFI18))
	{
		return FALSE;
	}

	if (!appWindow.InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	appWindow.Go();
 }




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK OnCommand(HWND hWnd, int id, HWND hwndCtrl, UINT codeNotify)
{
    switch (id)
    {
    case IDM_ABOUT:
        DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        return 0;
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        return 0;
        break;
    default:
        return DefWindowProc(hWnd, WM_COMMAND, id, codeNotify);
    }
    return (LRESULT)-1;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
