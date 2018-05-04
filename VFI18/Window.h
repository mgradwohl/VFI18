#pragma once
class Window
{
public:
	Window();
	~Window();

	//const HINSTANCE hInstance() const { return _hInstance; }
	//void hInstance(const HINSTANCE __hInstance) { _hInstance = __hInstance; }

protected:
	HWND _hWnd;
	HINSTANCE _hInstance;

};

