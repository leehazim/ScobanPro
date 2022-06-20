#include "MainWindow.h"

MainWindow* MainWindow::_Instance = nullptr;
LPCTSTR MainWindow::lpszClass = L"MainWindow";
HWND MainWindow::Tiles[MAX_HEIGHT][MAX_WIDTH];
ChildWindow* MainWindow::ChildInfo;

MainWindow::MainWindow() {
	
}

MainWindow::~MainWindow() {
	if(_Instance != nullptr)
		delete _Instance;
}

LRESULT MainWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc; PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		SetWindowPos(hwnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
		ChildInfo->Show();
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
	MSG Msg = { 0 };
	ChildInfo = new ChildWindow();
	ChildInfo->InitWindow(this->GetInstance(), 0);
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			 ChildInfo->MakeTile(j, i, 32, 32);
		}
	}
	while (GetMessage(&Msg, nullptr, 0, 0)) {
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
