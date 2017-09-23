#include "stdafx.h"
#include "window.h"

Window::Window()
{
}

Window::~Window()
{
}

void Window::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(_hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(_hWnd, &ps);
}

void Window::OnDestroy()
{
	PostQuitMessage(0);
}

bool Window::SetWindowClassAttributes(WORD idIcon, WORD idSmallIcon, WORD idMenu, WORD idAccelerators, WORD idClass, WORD idTitle)
{
	_hIcon = LoadIconW(_hInstance, MAKEINTRESOURCE(idIcon));
	if (_hIcon == NULL)
	{
		return false;
	}

	_hSmallIcon = LoadIconW(_hInstance, MAKEINTRESOURCE(idSmallIcon));
	if (_hIcon == NULL)
	{
		return false;
	}

	_hAccelerators = LoadAccelerators(_hInstance, MAKEINTRESOURCE(idAccelerators));
	if (_hAccelerators == NULL)
	{
		return false;
	}

	LPWSTR pszBuffer = nullptr;
	_szTitle.clear();
	if (size_t length = ::LoadString(_hInstance, idTitle, (LPWSTR)&pszBuffer, 0))
	{
		_szTitle.assign(pszBuffer, length);
	}
	else
	{
		return false;
	}

	_szWindowClass.clear();
	if (size_t length = ::LoadString(_hInstance, idClass, (LPWSTR)&pszBuffer, 0))
	{
		_szWindowClass.assign(pszBuffer, length);
	}
	else
	{
		return false;
	}

	_szMenuName.clear();
	if (size_t length = ::LoadString(_hInstance, idMenu, (LPWSTR)&pszBuffer, 0))
	{
		_szMenuName.assign(pszBuffer, length);
	}
	else
	{
		return false;
	}
}

bool Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	_hInstance = hInstance; // Store instance handle in our global variable

	if (RegisterClass() == 0)
	{
		return false;
	}

	_hWnd = CreateWindowW(_szWindowClass.c_str(), _szTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInstance, nullptr);
	if (!_hWnd)
	{
		return false;
	}

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	return true;
}

int Window::Go()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, _hAccelerators, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM Window::RegisterClass()
{
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = _hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = _szMenuName.c_str();
	wcex.lpszClassName = _szWindowClass.c_str();
	wcex.hIconSm = _hSmallIcon;

	return RegisterClassExW(&wcex);
}