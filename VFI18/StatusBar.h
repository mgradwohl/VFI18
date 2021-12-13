#pragma once

#include "Window.h"

class StatusBar : public Window
{
public:
	StatusBar() noexcept;
	~StatusBar();

	bool Initialize(HINSTANCE hInstance, HWND hWndParent, std::vector<float>& panes);
	bool Create();
	void Resize();
	bool UpdateFileCount(unsigned int i);
	bool UpdateTotalSize(uint64_t i);
	size_t GetHeight();


private:
	std::wstring _strIdle;
	std::vector<float> _panes;
};

