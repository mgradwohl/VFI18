#include "stdafx.h"
#include "ListView.h"
#include "WiseFile.h"
#include "wil/resource.h"

constexpr static size_t LIST_NUMCOLUMNS = STR_COLUMNLAST - STR_COLUMN0;
constexpr static size_t LIST_MAXHEADLENGTH = 20;

MyListView::MyListView()
{
	m_qwSize = 0;
}

MyListView::~MyListView()
{
	// get rid of everything
}

bool MyListView::AddFile(std::wstring& strFile)
{
	// *** if I do *** I get an access violation
	// *** because the shared_ptr gets cleaned up
	// *** std::shared_ptr<CWiseFile> pFile(new CWiseFile(strFile));
	// 
	//CWiseFile* pFile = new CWiseFile(strFile);
	auto pFile = new CWiseFile(strFile);

	m_qwSize += pFile->Size();
	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));

	int iItem = ListView_GetItemCount(_hWnd);
	item.iItem = iItem;
	item.iSubItem = 0;
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	item.state = 0;
	item.stateMask = 0;
	// *** access violation part two
	// item.lParam = (LPARAM)pFile.get();
	item.lParam = (LPARAM)pFile;
	item.pszText = LPSTR_TEXTCALLBACK;
	item.cchTextMax = 1024;

	iItem = ListView_InsertItem(_hWnd, &item);

	if (-1 == iItem)
	{
		TRACE(L">> Adding failed.\r\n");
		return false;
	}

	for (int iSubItem = 0; iSubItem < LIST_NUMCOLUMNS; iSubItem++)
	{
		ListView_SetItemText(_hWnd, iItem, iSubItem, LPSTR_TEXTCALLBACK);
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

	_hWnd = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT /*| LVS_EDITLABELS*/ | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE /*| LVS_OWNERDATA*/,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		_hWndParent, nullptr, hInstance, NULL);

	if (_hWnd == nullptr)
	{
		return false;
	}

	DWORD dwExStyle = ListView_GetExtendedListViewStyle(_hWnd);
	dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(_hWnd, dwExStyle);

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	InitColumns();

	if (!SetWindowSubclass(_hWnd, MyListView::StaticSubClass, 1, 0))
	{
		TRACE(L">> ListView subclass failed\r\n");
		return false;
	}

	TRACE(L">> ListView create succeeded\r\n");
	return true;
}

unsigned int MyListView::GetItemCount()
{
	return ListView_GetItemCount(_hWnd);
}

uint64_t MyListView::GetTotalSize()
{
	return m_qwSize;
}

LRESULT CALLBACK MyListView::StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	MyListView* pThis = (MyListView*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_NCDESTROY:
		TRACE(L">> Destroying ListView\r\n");
		RemoveWindowSubclass(hWnd, MyListView::StaticSubClass, uIdSubclass);
		return TRUE;

	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_GETDISPINFO:
		{
			TRACE(L">> ListView LVN_GETDISPINFO\r\n");
			NMLVDISPINFO* plvdi = reinterpret_cast<NMLVDISPINFO*>(lParam);
			pThis->OnGetDispInfo(plvdi);
			return TRUE;
		}
		case LVN_SETDISPINFO:
			TRACE(L">> ListView LVN_SETDISPINFO\r\n");
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);

		case LVN_GETINFOTIP:
			TRACE(L">> ListView LVN_GETINFOTIP\r\n");
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);

		case LVN_ITEMCHANGED:
			TRACE(L">> ListView LVN_ITEMCHANGED\r\n");
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);

		case LVN_INSERTITEM:
			TRACE(L">> ListView LVN_ITEMCHANGED\r\n");
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);
		default:
			return DefSubclassProc(hWnd, uMsg, wParam, lParam);
		}
	default: return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}

bool MyListView::InitColumns()
{
	LV_COLUMN lvc;
	ZeroMemory(&lvc, sizeof(lvc));
	wchar_t szText[LIST_MAXHEADLENGTH];

	lvc.pszText = szText;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (unsigned int iCol = 0; iCol < LIST_NUMCOLUMNS; iCol++)
	{
		lvc.iSubItem = iCol;
		LoadWstring(STR_COLUMN0 + iCol, szText, LIST_MAXHEADLENGTH);
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
	if (plvdi->item.mask & LVIF_TEXT)
	{
		CWiseFile* pFile = (CWiseFile*)(plvdi->item.lParam);
		//auto pFile = dynamic_cast<CWiseFile*>((plvdi->item.lParam));

		plvdi->item.pszText = pFile->GetFieldString(plvdi->item.iSubItem, false);
	}
	return true;
}

bool MyListView::Resize(size_t sbHeight)
{
	RECT rect;
	GetClientRect(_hWndParent, &rect);
	SetWindowPos(_hWnd, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top - sbHeight, SWP_NOZORDER);

	return true;
}
