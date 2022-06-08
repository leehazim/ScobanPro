#pragma once
#include <Windows.h>
#include "resource.h"

class MainWindow {
private:
	HWND hMainWnd;
	WNDCLASS WndClass;
	HINSTANCE hInstance;

	HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
	HWND hList;
	HWND hStatic;
	
	MainWindow* _instance;

public:
	
};

