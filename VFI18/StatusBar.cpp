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

bool StatusBar::UpdateFileCount(unsigned int i)
{
	wchar_t sz[100];
	wsprintf(sz, L"%u files", i);
	SendMessage(_hWnd, SB_SETTEXT, 1, (LPARAM)sz);
	return true;
}

bool StatusBar::UpdateTotalSize(unsigned int i)
{
	wchar_t szSize[100];
	int2str(szSize, i);

	wchar_t szStr[100];
	wsprintf(szStr, L"%s bytes", szSize);

	SendMessage(_hWnd, SB_SETTEXT, 1, (LPARAM)szStr);
	return true;
}

size_t StatusBar::GetHeight()
{
	RECT rectStatusBar;
	GetWindowRect(_hWnd, &rectStatusBar);

	return rectStatusBar.bottom - rectStatusBar.top;
}

bool StatusBar::Initialize(HINSTANCE hInstance, HWND hWndParent, std::vector<float>& panes)
{
	_hInstance = hInstance;
	_panes = panes;
	_hWndParent = hWndParent;
	LPWSTR pszBuffer = nullptr;

	_strIdle.clear();
	if (size_t length = ::LoadStringW(_hInstance, STRING_STATUSIDLE, (LPWSTR)&pszBuffer, 0))
	{
		_strIdle.assign(pszBuffer, length);
	}

	return true;
}

bool StatusBar::Create()
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_BAR_CLASSES;
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

void StatusBar::Resize()
{
	RECT rectParent;
	GetClientRect(_hWndParent, &rectParent);

	RECT rectStatusBar;
	GetWindowRect(_hWnd, &rectStatusBar);

	int Width = rectParent.right - rectParent.left; 

	std::vector<int> panes;
	int w = 0;
	for (std::vector<float>::iterator it = _panes.begin(); it != _panes.end(); ++it)
	{
		w += *it * (int)Width;
		panes.push_back(w);
	}

	SendMessage(_hWnd, SB_SETPARTS, _panes.size(), (LPARAM)&panes[0]);
	LPARAM lpsize = MAKELPARAM(Width, rectStatusBar.bottom - rectStatusBar.top);
	SendMessage(_hWnd, WM_SIZE, 0, lpsize);
}