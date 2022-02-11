#pragma once

//#include <stdint.h>
#include "window.h"

typedef std::shared_ptr<CWiseFile> spWiseFile;
typedef std::shared_ptr<LV_ITEM> spLVItem;

class MyListView : public Window
{
public:
	MyListView() noexcept;
	~MyListView();

	bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	unsigned int GetItemCount();
	uint64_t GetTotalSize();
	bool AddFile(const std::wstring& strFile);

private:
	static LRESULT CALLBACK StaticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	bool InitColumns();
	bool OnGetDispInfo(NMLVDISPINFO* plvdi);
	uint64_t m_qwSize;

public:
	bool Resize(size_t sbHeight);
};

