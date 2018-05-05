#include "stdafx.h"
#include "ListView.h"
#include "WiseFile.h"

#define LIST_NUMCOLUMNS	5
#define LIST_MAXHEADLENGTH 20

bool MyListView::AddFile(std::wstring& strFile)
{
	spWiseFile pFile(new CWiseFile(strFile));
	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));

	int iItem = ListView_GetItemCount(_hWnd);
	item.iItem = iItem;
	item.iSubItem = 0;
	item.mask = LVIF_TEXT | LVIF_PARAM;// | LVIF_DI_SETITEM;
	item.lParam = (LPARAM) pFile.get();
	item.cchTextMax = 1024;

// test
	//std::wstring path = pFile->GetFullPath();
	//LPWSTR pszPath = &path[0];
	//item.pszText = pszPath; //LPSTR_TEXTCALLBACK;

	item.pszText = LPSTR_TEXTCALLBACK;

	iItem = ListView_InsertItem(_hWnd, &item);

	if (-1 == iItem)
	{
		TRACE(L">> Adding failed.\r\n");
		return false;
	}

	return true;
}

bool MyListView::RegisterCreate(HINSTANCE hInstance, HWND hWnd)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	if (hWnd == NULL)
	{
		// must have a parent
		return false;
	}
	_hWndParent = hWnd;

	RECT rcClient;
	if (0 == GetClientRect(_hWndParent, &rcClient))
	{
		return false;
	}

	_hWnd = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT /*| LVS_EDITLABELS*/ | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE /*| LVS_OWNERDATA*/,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		_hWndParent, nullptr, hInstance, NULL);

	if (_hWnd == nullptr)
	{
		return false;
	}
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	InitColumns();

	DWORD dwExStyle = ListView_GetExtendedListViewStyle(_hWnd);
	dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(_hWnd, dwExStyle);

	if (!SetWindowSubclass(_hWnd, MyListView::StaticSubClass, 1, 0))
	{
		TRACE(L">> ListView subclass failed\r\n");
		return false;
	}

	//COLORREF color = RGB(192, 192, 255);
	//ListView_SetBkColor(_hWnd, color);

	ShowWindow(_hWnd, SW_SHOW);
	TRACE(L">> ListView create succeeded\r\n");
	return true;
}

unsigned int MyListView::GetItemCount()
{
	return ListView_GetItemCount(_hWnd);
}

LRESULT CALLBACK MyListView::StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	MyListView *pThis = (MyListView*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case LVN_GETDISPINFO:
				{
					NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
					pThis->OnGetDispInfo(plvdi);
					return TRUE;
				}
			}
		}

		case WM_NCDESTROY:
		{
			TRACE(L">> Destroying ListView\r\n");
			//RemoveWindowSubclass(hWnd, MyListView::StaticSubClass, uIdSubclass);
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

bool MyListView::InitColumns()
{
	LV_COLUMN lvc;
	ZeroMemory(&lvc, sizeof(lvc));
	WCHAR szText[LIST_MAXHEADLENGTH];

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (unsigned int iCol = 0; iCol < LIST_NUMCOLUMNS; iCol++)
	{
		lvc.pszText = szText;
		wsprintf(szText, L"Column %u", iCol);
		lvc.iSubItem = iCol;
		lvc.cx = 100;
		if (-1 == ListView_InsertColumn(_hWnd, iCol, &lvc))
		{
			TRACE(L">> Add column failed\r\n");
		}
	}

	return true;
}

bool MyListView::OnGetDispInfo(NMLVDISPINFO* plvdi)
{
	CWiseFile* pFile = (CWiseFile*)plvdi->item.lParam;

	std::wstring tmp = pFile->GetName();
	LPWSTR ptmp = &tmp[0];

	return true;
}

//void MyListView::OnPaint()
//{
//	PAINTSTRUCT ps;
//	HDC hdc = BeginPaint(_hWnd, &ps);
//	// TODO: Add any drawing code that uses hdc here...
//	EndPaint(_hWnd, &ps);
//}

bool MyListView::Resize()
{
	RECT rect;
	GetClientRect(_hWndParent, &rect);
	SetWindowPos(_hWnd, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
	return true;
}
