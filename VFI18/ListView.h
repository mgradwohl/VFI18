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
	bool OnGetDispInfo(NMLVDISPINFO* plvdi);

public:
	bool Resize(int sbHeight);
};

