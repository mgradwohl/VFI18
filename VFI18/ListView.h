#pragma once
#include "window.h"

class MyListView : public Window
{
public:
	bool RegisterCreate(HINSTANCE hInstance, HWND hWnd);
	bool CreateChildren();

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnPaint();
};

