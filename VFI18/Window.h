#include "StatusBar.h"

class Window
{
public:
	Window(HINSTANCE hInstance);
	~Window();

	// 
	bool SetWindowClassAttributes(WORD idIcon, WORD idSmallIcon, WORD idMenu, WORD idAccelerators, WORD idClass, WORD idTitle);
	bool InitInstance(HINSTANCE hInstance, int nCmdShow);
	int Go();

	// WM_ handlers
	virtual void OnPaint();
	virtual void OnDestroy();
	void OnSize(int Width);
	void OnFileAdd();
	
	// helpers
	int ErrorMessageBox(const DWORD dwError, std::wstring szMessage);

private:
	bool RegisterClass();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	// handlers
	LRESULT CALLBACK OnCommand(HWND hwndCtrl, int id, UINT codeNotify);

	// members
	StatusBar _statusbar;
	MyList _list;
	HWND _hWnd;
	HINSTANCE _hInstance;
	std::wstring _szTitle;
	std::wstring _szWindowClass;
	std::wstring _szMenuName;
	HMENU _hMenu;
	HICON _hIcon;
	HICON _hSmallIcon;
	HACCEL _hAccelerators;
};