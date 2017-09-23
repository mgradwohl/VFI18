// VFI18.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "VFI18.h"
#include "window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Window appWindow(hInstance);

	if (!appWindow.SetWindowClassAttributes(IDI_VFI18, IDI_SMALL, IDC_VFI18, IDC_VFI18, IDS_APP_TITLE, IDC_VFI18))
	{
		return FALSE;
	}

	if (!appWindow.InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	return appWindow.Go();
 }