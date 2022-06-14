#pragma once
#include <Windows.h>
class BaseWindow {
public:
	BaseWindow();
	virtual ~BaseWindow();

	void SetInstance(HINSTANCE hInstance);
	HINSTANCE GetInstance();
	HWND GetHandleWnd();

	virtual bool InitWindow(HINSTANCE, int) = 0;

protected:
	WNDCLASS WndClass;
	static HINSTANCE m_hInst;
	static HWND m_hWnd;
};

