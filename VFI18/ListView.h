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
	static LRESULT CALLBACK StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	bool InitColumns();
	bool OnNotify(HWND hWnd, unsigned int Code);
	bool OnGetDispInfo(NMLVDISPINFO* plvdi);
	WNDPROC prevWndProc;

	//void OnPaint();
public:
	bool Resize();
};

