#include "MainWindow.h"

MainWindow* MainWindow::_Instance;
BitmapManager* MainWindow::Bit_Instance;
GameManager* MainWindow::Game_Instance;

MainWindow::MainWindow() 
	: BaseWindow(nullptr), lpszClass(L"MainWindow") {}

MainWindow::MainWindow(HINSTANCE hInstance) 
	: BaseWindow(hInstance), lpszClass(L"MainWindow") {}

MainWindow::~MainWindow() {
	delete Bit_Instance;
}

LRESULT MainWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc; PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		SetWindowPos(hwnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
		InitBitManager();
		InitGameManager();
		Bit_Instance->LoadBitFile(hwnd);
		Game_Instance->LoadMap();
		return 0;

	case WM_TIMER:
		if (Game_Instance->CheckClear()) {
			KillTimer(hwnd, 1);
			if (MessageBox(hwnd, L"클리어!\n 다음스테이지로 이동합니다.", L"축하합니다!", MB_OKCANCEL) == IDOK) {
				if (Game_Instance->SetStage(Game_Instance->GetStage() + 1))
					return 0;
				SendMessage(hwnd, MESSAGE_START, 0, 0);
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Game_Instance->Render(hdc);
		EndPaint(hwnd, &ps);
		return 0;

	case MESSAGE_START:
		Game_Instance->InitStage();
		SetTimer(hwnd, 1, 10, NULL);
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'S':
			SendMessage(hwnd, MESSAGE_START, 0, 0);
		}
		Game_Instance->Move(wParam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

void MainWindow::InitBitManager() {
	Bit_Instance = new BitmapManager();
}

void MainWindow::InitGameManager() {
	Game_Instance = new GameManager();
}

bool MainWindow::InitWindow() {
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	RegisterClass(&WndClass);

	m_hWnd = CreateWindow(lpszClass, L"Scoban", WS_OVERLAPPEDWINDOW,
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

BitmapManager* MainWindow::GetBitmapManager() {
	return Bit_Instance;
}

GameManager* MainWindow::GetGameInstance() {
	return Game_Instance;
}
