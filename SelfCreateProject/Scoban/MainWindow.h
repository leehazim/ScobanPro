#pragma once
#include "BaseWindow.h"

class MainWindow 
		:public BaseWindow {
public:
	MainWindow();
	MainWindow(HINSTANCE hInstance);
	
	bool InitWindow();
	void Active(int nCmdShow);
	static MainWindow* GetInstance(HINSTANCE);

private:	
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	static MainWindow* _Instance;
	LPCTSTR lpszClass; 
};