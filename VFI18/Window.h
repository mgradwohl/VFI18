#pragma once

class Window
{
public:
	Window();
	~Window();

	HWND _hWnd;
	HWND GetHWnd();

protected:
	HWND _hWndParent;
	HINSTANCE _hInstance;
};

