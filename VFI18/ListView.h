#pragma once

#include "window.h"

typedef std::shared_ptr<CWiseFile> spWiseFile;
typedef std::shared_ptr<LV_ITEM> spLVItem;

class MyListView : public Window
{
public:
	MyListView();
	~MyListView();

	bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	unsigned int GetItemCount();
	unsigned int GetTotalSize();
	bool AddFile(std::wstring& strFile);

private:
	static LRESULT CALLBACK StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	bool InitColumns();
	bool OnGetDispInfo(NMLVDISPINFO* plvdi);
	DWORD m_qwSize;

public:
	bool Resize(size_t sbHeight);
};

