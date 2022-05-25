#pragma once
#include "BaseWindow.h"
#include "BitmapManager.h"

class MainWindow
	:public BaseWindow {
public:
	MainWindow();
	MainWindow(HINSTANCE hInstance);

	bool InitWindow();
	void Active(int nCmdShow);
	static MainWindow* GetSingleInstance();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void InitBitManager();

private:
	static BitmapManager* Bit_Instance;
	static MainWindow* _Instance;
	LPCTSTR lpszClass;

};