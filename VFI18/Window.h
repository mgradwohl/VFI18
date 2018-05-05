#pragma once

class Window
{
public:
	Window();
	~Window();

protected:
	HWND _hWnd;
	HWND _hWndParent;
	HINSTANCE _hInstance;
};

