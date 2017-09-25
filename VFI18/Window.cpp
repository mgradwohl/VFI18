#include "stdafx.h"
#include "window.h"
#include "AboutBox.h"
#include "StatusBar.h"

Window::Window(HINSTANCE hInstance)
{
	_hInstance = hInstance;
}

Window::~Window()
{
}

LRESULT Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window *pThis = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (pThis != NULL)
	{
		return pThis->WndProc(hWnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			OnPaint();
			break;
		}
		case WM_DESTROY:
		{
			OnDestroy();
			break;
		}
		case WM_SIZE:
		{
			OnSize(LOWORD(lParam));
			break;
		}
		case WM_COMMAND:
		{
			OnCommand((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
		}
		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	};
	return 0;
}

void Window::OnSize(int Width)
{
	_statusbar.Resize(Width);
}

LRESULT CALLBACK Window::OnCommand(HWND hwndCtrl, int id, UINT codeNotify)
{
	switch (id)
	{
		case IDM_ABOUT:
		{
			AboutBox aboutBox;
			aboutBox.Create(_hInstance, _hWnd);
			return 0;
		}
		case IDM_EXIT:
		{
			DestroyWindow(_hWnd);
			return 0;
		}
		default:
		return DefWindowProc(_hWnd, WM_COMMAND, id, codeNotify);
	}
	return (LRESULT)-1;
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
	if (idMenu != 0)
	{
		if (size_t length = ::LoadString(_hInstance, idMenu, (LPWSTR)&pszBuffer, 0))
		{
			_szMenuName.assign(pszBuffer, length);
		}
		else
		{
			return false;
		}

		_hMenu = ::LoadMenu(_hInstance, MAKEINTRESOURCE(idMenu));
		if (_hMenu == NULL)
		{
			ErrorMessageBox(::GetLastError(), L"LoadMenu failed");
			return false;
		}
	}

	return true;
}

bool Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	if (!RegisterClass())
	{
		return false;
	}

	_hWnd = CreateWindowW(_szWindowClass.c_str(), _szTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, _hMenu, _hInstance, nullptr);
	if (!_hWnd)
	{
		return false;
	}

	// store the this pointer so Window::WndProc doesn't need to be static
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	// Setup the status bar with 3 panes, 1: 50%, 2: 25%, 3: 25%
	vector<float> panes({ 0.50, 0.25, 0.25 });
	_statusbar.Initialize(_hInstance, _hWnd, panes);
	_statusbar.Create();

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

bool Window::RegisterClass()
{
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &Window::StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = _hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = _szMenuName.c_str();
	wcex.lpszClassName = _szWindowClass.c_str();
	wcex.hIconSm = _hSmallIcon;

	if (RegisterClassExW(&wcex) == 0)
	{
		return false;
	}
	return true;
}

int Window::ErrorMessageBox(const DWORD dwError, wstring szMessage)
{
	LPWSTR pBuffer = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pBuffer, 0, NULL);

	const wstring errortext(L"%s\r\n\r\nError number: %lu\r\n\r\n%s");
	const size_t bufferlength = szMessage.length() + lstrlen(pBuffer) + errortext.length() + 1;

	wstring buffer;
	buffer.resize(bufferlength);

	wsprintf(&buffer[0], errortext.c_str(), szMessage.c_str(), dwError, pBuffer);
	LocalFree(pBuffer);

	const int error = MessageBox(_hWnd, buffer.c_str(), _szTitle.c_str(), MB_OK | MB_ICONINFORMATION);
	return error;
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