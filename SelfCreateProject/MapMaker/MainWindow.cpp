#include "MainWindow.h"

MainWindow* MainWindow::_Instance;
BitmapManager* MainWindow::Bit_Instance;

MainWindow::MainWindow() 
	: BaseWindow(nullptr), lpszClass(L"MainWindow") {}

MainWindow::MainWindow(HINSTANCE hInstance) 
	: BaseWindow(hInstance), lpszClass(L"MainWindow") {}

MainWindow::~MainWindow() {
	if (Bit_Instance != nullptr)
		delete Bit_Instance;

	if(_Instance != nullptr)
		delete _Instance;
}

LRESULT MainWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc; PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		SetWindowPos(hwnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
		return 0;

	case WM_TIMER:

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_KEYDOWN:
	
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

bool MainWindow::InitWindow() {
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	RegisterClass(&WndClass);

	m_hWnd = CreateWindow(lpszClass, L"Map Maker", WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  0, 0, NULL, (HMENU)NULL, m_hInst, NULL);
	if (m_hWnd == NULL) 
		return false;
	
	return true;
}

void MainWindow::Active(int nCmdShow) {
	ShowWindow(m_hWnd, nCmdShow);
}

MainWindow* MainWindow::GetSingleInstance() {
	if (_Instance == nullptr)
		_Instance = new MainWindow();
	return _Instance;
}
