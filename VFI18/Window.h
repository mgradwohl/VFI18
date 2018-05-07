#pragma once

class Window
{
public:
	Window();
	~Window();

	HWND _hWnd;

protected:
	HWND _hWndParent;
	HINSTANCE _hInstance;
};

