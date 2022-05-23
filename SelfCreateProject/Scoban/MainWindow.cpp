#include "MainWindow.h"

MainWindow* MainWindow::_Instance;

MainWindow::MainWindow() {}
MainWindow::MainWindow(HINSTANCE hInstance) 
	: BaseWindow(hInstance), lpszClass(L"MainWindow") {}

LRESULT MainWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	
	switch (iMessage) {
	case WM_CREATE:
		SetWindowPos(hwnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
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

	m_hWnd = CreateWindow(lpszClass, L"Scoban", WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  0, 0, NULL, (HMENU)NULL, m_hInst, NULL);
	if (m_hWnd == NULL) {
		return false;
	}

	return true;
}

void MainWindow::Active(int nCmdShow) {
	ShowWindow(m_hWnd, nCmdShow);
}

MainWindow* MainWindow::GetInstance(HINSTANCE hInstance) {
	if (_Instance == nullptr) 
		_Instance= new MainWindow(hInstance);
	return _Instance;
}