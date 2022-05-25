#pragma once
#include <Windows.h>
class BaseWindow {
protected:
	WNDCLASS WndClass;
	HINSTANCE m_hInst;
	HWND m_hWnd;

public:
	BaseWindow();
	BaseWindow(HINSTANCE hInstance);
	virtual ~BaseWindow();

	void SetInstance(HINSTANCE hInstance);
	HINSTANCE GetInstance();
	HWND GetHandleWnd();

	virtual bool InitWindow() = 0;
};

