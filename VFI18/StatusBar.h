#pragma once
class StatusBar
{
public:
	StatusBar();
	~StatusBar();

	bool Initialize(HINSTANCE hInstance, HWND hWndParent, vector<float>& panes);
	bool Create();
	void Resize(int Width);

private:
	HWND _hWnd;
	HWND _hWndParent;
	HINSTANCE _hInstance;
	wstring _strIdle;
	vector<float> _panes;
};

