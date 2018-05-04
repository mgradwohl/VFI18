#pragma once
#include "Window.h"
class StatusBar : public Window
{
public:
	StatusBar();
	~StatusBar();

	bool Initialize(HINSTANCE hInstance, HWND hWndParent, std::vector<float>& panes);
	bool Create();
	void Resize(int Width);

private:
	HWND _hWnd;
	HWND _hWndParent;
	HINSTANCE _hInstance;
	std::wstring _strIdle;
	std::vector<float> _panes;
};

