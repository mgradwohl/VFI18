#pragma once
class StatusBar
{
public:
	StatusBar();
	~StatusBar();

	bool Create(HWND hWndParent, HINSTANCE hInstance);
	void Resize(int Width);

private:
	HWND _hWnd;
};

