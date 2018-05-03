#pragma once

//forwards
class StatusBar;
class MyListView;

class Window
{
public:
	Window(HINSTANCE hInstance);
	~Window();

	// 
	bool SetWindowClassAttributes(CLASSATTRIBS* classattribs);
	virtual bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	virtual bool CreateChildren();
	int Go(int nCmdShow);

	// WM_ handlers
	virtual void OnPaint();
	void OnDestroy();
	void OnSize(int Width);
	void OnFileAdd();
	
	// helpers
	int ErrorMessageBox(const DWORD dwError, std::wstring szMessage);

protected:
	HWND _hWnd;
	HINSTANCE _hInstance;

private:
	bool RegisterClass();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	// handlers
	LRESULT CALLBACK OnCommand(HWND hwndCtrl, int id, UINT codeNotify);

	//list of child windows
	std::vector<Window> children;
	//StatusBar _statusbar;
	MyList _list;

	std::wstring _szTitle;
	std::wstring _szWindowClass;
	std::wstring _szMenuName;

	HMENU _hMenu;
	HICON _hIcon;
	HICON _hSmallIcon;
	HACCEL _hAccelerators;
};