#pragma once

#include "window.h"

typedef std::shared_ptr<CWiseFile> spWiseFile;
typedef std::shared_ptr<LV_ITEM> spLVItem;

class MyListView : public Window
{
public:
	bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	unsigned int GetItemCount();
	bool AddFile(std::wstring& strFile);

private:
	static LRESULT CALLBACK MyListView::StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	bool MyListView::InitColumns();
	bool MyListView::OnGetDispInfo(NMLVDISPINFO* plvdi);
	WNDPROC prevWndProc;

	//void OnPaint();
public:
	bool Resize();
};

