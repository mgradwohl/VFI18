class Window
{
public:
	Window(HINSTANCE hInstance);
	~Window();
	HWND GetHandle()
	{
		return _hWnd;
	};
	virtual void OnPaint();
	virtual void OnDestroy();
	bool SetWindowClassAttributes(WORD idIcon, WORD idSmallIcon, WORD idMenu, WORD idAccelerators, WORD idClass, WORD idTitle);
	bool InitInstance(HINSTANCE hInstance, int nCmdShow);
	int Go();

	int ErrorMessageBox(const DWORD dwError, wstring szMessage);


private:
	HWND _hWnd;
	HINSTANCE _hInstance;
	wstring _szTitle;
	wstring _szWindowClass;
	wstring _szMenuName;
	HMENU _hMenu;
	HICON _hIcon;
	HICON _hSmallIcon;
	HACCEL _hAccelerators;

	ATOM RegisterClass();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};