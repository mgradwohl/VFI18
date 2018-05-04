#include "stdafx.h"
#include "ListView.h"

bool MyListView::RegisterCreate(HINSTANCE hInstance, HWND hWnd)
{
	if (hWnd == NULL)
	{
		// must have a parent
		return false;
	}

	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;
	if (0 == GetClientRect(hWnd, &rcClient))
		return false;

	_hWnd = CreateWindowW(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hWnd, nullptr, hInstance, NULL);

	if (_hWnd == nullptr)
		return false;

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	return true;
}

LRESULT CALLBACK MyListView::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		OnPaint();
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	};
	return 0;
}

void MyListView::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(_hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(_hWnd, &ps);
}

bool MyListView::CreateChildren()
{
	return true;
}
