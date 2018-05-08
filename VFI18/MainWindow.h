#pragma once

#include "Window.h"
#include "StatusBar.h"
#include "ListView.h"

class MainWindow : public Window
{
public:
	MainWindow(HINSTANCE hInstance);
	~MainWindow();

	bool SetWindowClassAttributes(CLASSATTRIBS* classattribs);
	virtual bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	virtual bool CreateChildren();
	int Go(int nCmdShow);

	// WM_ handlers
	virtual void OnPaint();
	void OnDestroy();
	void OnSize();
	void OnFileAdd();
	
	// helpers
	int ErrorMessageBox(const DWORD dwError, std::wstring szMessage);

private:
	bool RegisterClass();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// handlers
	LRESULT CALLBACK OnCommand(HWND hwndCtrl, int id, UINT codeNotify);

	//list of child windows
	StatusBar _statusbar;
	MyListView _listview;
	//MyList _list;

	std::wstring _szTitle;
	std::wstring _szWindowClass;
	std::wstring _szMenuName;

	HMENU _hMenu;
	HICON _hIcon;
	HICON _hSmallIcon;
	HACCEL _hAccelerators;
};