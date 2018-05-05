#include "stdafx.h"
#include "ListView.h"
#include "WiseFile.h"

#define LIST_NUMCOLUMNS	5
#define LIST_MAXHEADLENGTH 20

bool MyListView::AddFile(std::wstring& strFile)
{
	spWiseFile pFile(new CWiseFile(strFile));
	spLVItem pItem(new LVITEM);

	//CWiseFile* pFile = new (CWiseFile);
	pItem->lParam = (LPARAM) pFile.get();

	int iItem = ListView_GetItemCount(_hWnd);
	pItem->iItem = iItem;

	pItem->iSubItem = 0;
	pItem->mask = LVIF_TEXT | LVIF_PARAM | LVIF_DI_SETITEM;
	pItem->pszText = LPSTR_TEXTCALLBACK;
	pItem->cchTextMax = 1024;

	iItem = ListView_InsertItem(_hWnd, pItem.get());

	int iSubItem = 1;
	if (-1 == iItem)
	{
		//adding failed
		return false;
	}
	else
	{
		for (iSubItem = 1; iSubItem < LIST_NUMCOLUMNS; iSubItem++)
		{

			ListView_SetItemText(_hWnd, iItem, iSubItem, LPSTR_TEXTCALLBACK)
		}
	}

	ListView_Update(_hWnd, iItem);

	return true;
}

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

	_hWnd = CreateWindowW(WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hWnd, nullptr, hInstance, NULL);

	if (_hWnd == nullptr)
		return false;

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	DWORD dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	dwExStyle &= ~WS_EX_DLGMODALFRAME;
	ListView_SetExtendedListViewStyle(_hWnd, dwExStyle);

	SetWindowSubclass(_hWnd, MyListView::StaticSubClass, 1, 0);

	InitColumns();

	ShowWindow(_hWnd, SW_SHOW);

	return true;
}

LRESULT CALLBACK MyListView::StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_NCDESTROY) {
		// NOTE: this requirement is NOT stated in the documentation,
		// but it is stated in Raymond Chen's blog article...
		RemoveWindowSubclass(hWnd, MyListView::StaticSubClass, uIdSubclass);
	}

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
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

bool MyListView::InitColumns()
{
	LV_COLUMN lvc;
	ZeroMemory(&lvc, sizeof(lvc));
	WCHAR szText[LIST_MAXHEADLENGTH];

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.pszText = szText;

	for (unsigned int iCol = 0; iCol < LIST_NUMCOLUMNS; iCol++)
	{
		wsprintf(szText, L"Column %u", iCol);
		lvc.iSubItem = iCol;

		if (-1 == ListView_InsertColumn(_hWnd, iCol, &lvc))
		{
			TRACE(L"Add column failed");
		}
	}

	return true;
}

bool MyListView::OnGetDispInfo(NMLVDISPINFO* plvdi)
{
	// get the item
	//LVITEM* pItem = plvdi->item;
	CWiseFile* pFile = (CWiseFile*)plvdi->item.lParam;

	std::wstring tmp = pFile->GetName();
	LPWSTR ptmp = &tmp[0];

	plvdi->item.pszText = ptmp;

	return true;
}

//LRESULT CALLBACK MyListView::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch (msg)
//	{
//		//case WM_PAINT:
//		//{
//		//	OnPaint();
//		//	break;
//		//}
//		case WM_NOTIFY:
//		{
//			switch (((LPNMHDR)lParam)->code)
//			{
//				case LVN_GETDISPINFO:
//				{
//					NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
//					OnGetDispInfo(plvdi);
//					break;
//				}
//			}
//		}
//		default:
//		{
//			return DefWindowProc(hWnd, msg, wParam, lParam);
//		}
//	};
//
//	return 0;
//}

//void MyListView::OnPaint()
//{
//	PAINTSTRUCT ps;
//	HDC hdc = BeginPaint(_hWnd, &ps);
//	// TODO: Add any drawing code that uses hdc here...
//	EndPaint(_hWnd, &ps);
//}

bool MyListView::Resize()
{
	HWND hWndParent = GetParent(_hWnd);
	RECT rect;
	GetClientRect(hWndParent, &rect);

	// TODO: Add your implementation code here.
	return true;
}
