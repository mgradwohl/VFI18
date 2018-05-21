#include "stdafx.h"
#include "mainwindow.h"
#include "DialogBox.h"
#include "StatusBar.h"

MainWindow::MainWindow(HINSTANCE hInstance)
{
	_hInstance = hInstance;
}

MainWindow::~MainWindow()
{
}

LRESULT MainWindow::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MainWindow *pThis = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (pThis != nullptr)
	{
		return pThis->WndProc(hWnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
		case WM_DESTROY:
		{
			OnDestroy();
			break;
		}
		case WM_NOTIFY:
		{
			TRACE(L">> MainWindow forward WM_NOTIFY\r\n");
			//messages are sent to the parent, because Win32 assumes the parent will handle these
			//I forward them to the window that needs them, because the code to handle these
			//is in the same class that owns the _hWnd that sent the message
			LPNMHDR pHdr = (LPNMHDR)lParam;
			{
				FORWARD_WM_NOTIFY(pHdr->hwndFrom, wParam, pHdr/*(NMHDR*)lParam*/, SendMessage);
			}
			break;
		}

		case WM_SIZE:
		{
			OnSize();
			break;
		}
		case WM_COMMAND:
		{
			return OnCommand((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
		}
		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	};
	return 0;
}

void MainWindow::OnSize()
{
	// resize all children
	_statusbar.Resize();

	_listview.Resize(_statusbar.GetHeight());
}

void MainWindow::OnFileAdd()
{
	std::wstring strFile;
	OpenBox(_hWnd, L"Choose a file", L"*.*", strFile, NULL, OFN_FILEMUSTEXIST | OFN_FORCESHOWHIDDEN | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY);

	_listview.AddFile(strFile);
	_statusbar.UpdateFileCount(_listview.GetItemCount());
}

LRESULT CALLBACK MainWindow::OnCommand(HWND hwndCtrl, int id, UINT codeNotify)
{
	switch (id)
	{
	case ID_FILE_ADD:
		{
			OnFileAdd();
			return 0;
		}
		case IDM_ABOUT:
		{
			MyDialogBox aboutBox;
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

bool MainWindow::SetWindowClassAttributes(CLASSATTRIBS* classattribs)
{
	_hIcon = LoadIconW(_hInstance, MAKEINTRESOURCE(classattribs->idIcon));
	if (_hIcon == NULL)
	{
		return false;
	}

	_hSmallIcon = LoadIconW(_hInstance, MAKEINTRESOURCE(classattribs->idSmallIcon));
	if (_hIcon == NULL)
	{
		return false;
	}

	_hAccelerators = LoadAccelerators(_hInstance, MAKEINTRESOURCE(classattribs->idAccelerators));
	if (_hAccelerators == NULL)
	{
		return false;
	}

	LPWSTR pszBuffer = nullptr;
	_szTitle.clear();
	if (size_t length = ::LoadString(_hInstance, classattribs->idTitle, (LPWSTR)&pszBuffer, 0))
	{
		_szTitle.assign(pszBuffer, length);
	}
	else
	{
		return false;
	}

	_szWindowClass.clear();
	if (size_t length = ::LoadString(_hInstance, classattribs->idClass, (LPWSTR)&pszBuffer, 0))
	{
		_szWindowClass.assign(pszBuffer, length);
	}
	else
	{
		return false;
	}

	_szMenuName.clear();
	if (classattribs->idMenu != 0)
	{
		_hMenu = ::LoadMenu(_hInstance, MAKEINTRESOURCE(classattribs->idMenu));
		if (_hMenu == NULL)
		{
			ErrorMessageBox(::GetLastError(), L"LoadMenu failed");
			return false;
		}
	}

	return true;
}

bool MainWindow::RegisterCreate(HINSTANCE hInstance, HWND hWnd)
{
	if (!RegisterClass())
	{
		return false;
	}
	_hWndParent = hWnd;
	_hWnd = CreateWindowW(_szWindowClass.c_str(), _szTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd, _hMenu, _hInstance, nullptr);
	if (!_hWnd)
	{
		return false;
	}

	// store the this pointer so Window::WndProc doesn't need to be static
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	return true;
}

bool MainWindow::CreateChildren()
{
	// Setup the status bar with 3 panes, 1: 50%, 2: 25%, 3: 25%
	std::vector<float> panes({ 0.50, 0.25, 0.25 });
	
	if (!_statusbar.Initialize(_hInstance, _hWnd, panes))
	{
		return false;
	}

	if (!_statusbar.Create())
	{
		return false;
	}

	if (!_listview.RegisterCreate(_hInstance, _hWnd))
	{
		return false;
	}

	return true;
}

int MainWindow::Go(int nCmdShow)
{
	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

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

bool MainWindow::RegisterClass()
{
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &MainWindow::StaticWndProc;
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

int MainWindow::ErrorMessageBox(const DWORD dwError, std::wstring szMessage)
{
	LPWSTR pBuffer = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pBuffer, 0, NULL);

	const std::wstring errortext(L"%s\r\n\r\nError number: %lu\r\n\r\n%s");
	const size_t bufferlength = szMessage.length() + lstrlen(pBuffer) + errortext.length() + 1;

	std::wstring buffer;
	buffer.resize(bufferlength);

	wsprintf(&buffer[0], errortext.c_str(), szMessage.c_str(), dwError, pBuffer);
	LocalFree(pBuffer);

	const int error = MessageBox(_hWnd, buffer.c_str(), _szTitle.c_str(), MB_OK | MB_ICONINFORMATION);
	return error;
}

void MainWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(_hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(_hWnd, &ps);
}

void MainWindow::OnDestroy()
{
	PostQuitMessage(0);
}