class Window
{
public:
	Window();
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

private:
	HWND _hWnd;
	HINSTANCE _hInstance;
	wstring _szTitle;
	wstring _szWindowClass;
	wstring _szMenuName;
	HICON _hIcon;
	HICON _hSmallIcon;
	HACCEL _hAccelerators;

	ATOM RegisterClass();
};