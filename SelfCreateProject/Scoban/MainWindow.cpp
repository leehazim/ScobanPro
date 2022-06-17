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
		/*SetWindowPos(hwnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
		InitBitManager();
		InitGameManager();
		Bit_Instance->LoadBitFile(hwnd);
		Game_Instance->LoadMap();*/
		return 0;

	case WM_TIMER:
		/*if (Game_Instance->CheckClear()) {
			KillTimer(hwnd, 1);
			if (MessageBox(hwnd, L"클리어!\n 다음스테이지로 이동합니다.", L"축하합니다!", MB_OKCANCEL) == IDOK) {
				if (Game_Instance->SetStage(Game_Instance->GetStage() + 1))
					return 0;
				SendMessage(hwnd, MESSAGE_START, 0, 0);
			}
		}*/
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		/*Game_Instance->Render(hdc);*/
		EndPaint(hwnd, &ps);
		return 0;

	case MESSAGE_START:
		/*Game_Instance->InitStage();
		SetTimer(hwnd, 1, 10, NULL);*/
		return 0;

	case WM_KEYDOWN:
		/*switch (wParam) {
		case 'R':
			SendMessage(hwnd, MESSAGE_START, 0, 0);
		}
		Game_Instance->Move(wParam);*/
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

MSG MainWindow::Run() {
	
	MSG msg = { 0 };
	OnCreate();
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			OnUpdate();
		}
	}
	OnDestroy();
	return msg;
}

void MainWindow::OnCreate() {

	SetWindowPos(m_hWnd, NULL, 0, 0, 800, 640, SWP_NOMOVE);
	InitBitManager();
	InitGameManager();
	Bit_Instance->LoadBitFile(m_hWnd);
	Game_Instance->LoadMap();
	Game_Instance->InitStage();
	m_hDC = GetDC(m_hWnd);
}

void MainWindow::OnUpdate() {

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		Game_Instance->Move(VK_LEFT);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		Game_Instance->Move(VK_RIGHT);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		Game_Instance->Move(VK_UP);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		Game_Instance->Move(VK_DOWN);
	}
	Sleep(60);

	if (Game_Instance->CheckClear()) {
		OutputDebugString(L"Clear!");
		if (GetAsyncKeyState('R') & 0x8000) {
			Game_Instance->SetStage(Game_Instance->GetStage()+1);
			Game_Instance->InitStage();
		}
	}

	Game_Instance->Render(m_hDC);
}

void MainWindow::OnDestroy() {
	ReleaseDC(m_hWnd, m_hDC);
	delete Bit_Instance;
	delete Game_Instance;
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
