#include "stdafx.h"
#include "StatusBar.h"
#include "resource.h"

StatusBar::StatusBar()
{
	_hWnd = NULL;
	_panes.clear();
	_strIdle.clear();
}

StatusBar::~StatusBar()
{
}

bool StatusBar::Initialize(HINSTANCE hInstance, HWND hWndParent, vector<float>& panes)
{
	_hInstance = hInstance;
	_panes = panes;
	_hWndParent = hWndParent;
	LPWSTR pszBuffer = nullptr;

	_strIdle.clear();
	if (size_t length = ::LoadStringW(_hInstance, IDS_STATUSIDLE, (LPWSTR)&pszBuffer, 0))
	{
		_strIdle.assign(pszBuffer, length);
	}

	return true;
}
bool StatusBar::Create()
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&iccx);

	HMENU idStatus = 0;
	HWND hWndStatus = CreateWindowEx(0,	STATUSCLASSNAME, _strIdle.c_str(), SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, _hWndParent,	(HMENU)idStatus, _hInstance, NULL);
	if (hWndStatus == NULL)
	{
		return false;
	}
	_hWnd = hWndStatus;

	return true;
}

void StatusBar::Resize(int Width)
{
	vector<int> panes;
	int w = 0;
	for (vector<float>::iterator it = _panes.begin(); it != _panes.end(); ++it)
	{
		w += *it * Width;
		panes.push_back(w);
	}

	SendMessage(_hWnd, SB_SETPARTS, _panes.size(), (LPARAM)&panes[0]);
	SendMessage(_hWnd, WM_SIZE, 0, 0);
}