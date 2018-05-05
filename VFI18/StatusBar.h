#pragma once

#include "Window.h"

class StatusBar : public Window
{
public:
	StatusBar();
	~StatusBar();

	bool Initialize(HINSTANCE hInstance, HWND hWndParent, std::vector<float>& panes);
	bool Create();
	void Resize();
	bool UpdateFileCount(unsigned int i);
	unsigned int GetHeight();


private:
	std::wstring _strIdle;
	std::vector<float> _panes;
};

