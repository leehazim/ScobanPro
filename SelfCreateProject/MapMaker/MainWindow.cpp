#include "MainWindow.h"

MainWindow* MainWindow::_Instance;
BitmapManager* MainWindow::Bit_Instance;
LPCTSTR MainWindow::lpszClass = L"MainWindow";

MainWindow::MainWindow() {}

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

bool MainWindow::InitWindow(HINSTANCE hInstance, int nCmdShow) {
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.hInstance = hInstance;
	RegisterClass(&WndClass);

	m_hWnd = CreateWindow(lpszClass, L"Map Maker", WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  0, 0, NULL, (HMENU)NULL, m_hInst, NULL);
	if (m_hWnd == NULL) 
		return false;

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
	
	return true;
}

MSG MainWindow::Run() {
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg;
}

MainWindow* MainWindow::GetSingleInstance() {
	if (_Instance == nullptr)
		_Instance = new MainWindow();
	return _Instance;
}
