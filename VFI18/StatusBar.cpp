#include "stdafx.h"
#include "StatusBar.h"

StatusBar::StatusBar()
{
}

StatusBar::~StatusBar()
{
}

bool StatusBar::Create(HWND hWndParent, HINSTANCE hInstance)
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&iccx);

	HMENU idStatus = 0;
	// Create the status bar.
	HWND hWndStatus = CreateWindowEx(
		0,                       // no extended styles
		STATUSCLASSNAME,         // name of status bar class
		L"Ready",           // no text when first created
		SBARS_SIZEGRIP |         // includes a sizing grip
		WS_CHILD | WS_VISIBLE,   // creates a visible child window
		0, 0, 0, 0,              // ignores size and position
		hWndParent,              // handle to parent window
		(HMENU)idStatus,       // child window identifier
		hInstance,                   // handle to application instance
		NULL);                   // no window creation data

	if (hWndStatus == NULL)
	{
		return false;
	}
	_hWnd = hWndStatus;

	return true;
}

void StatusBar::Resize(int Width)
{
	// all this should be fixed

	int nWidths[3];
	nWidths[0] = 2 * Width / 4;
	nWidths[1] = 3 * Width / 4;
	nWidths[2] = 4 * Width / 4;
	SendMessage(_hWnd, SB_SETPARTS, 3, (LPARAM)nWidths);
	SendMessage(_hWnd, WM_SIZE, 0, 0);
}