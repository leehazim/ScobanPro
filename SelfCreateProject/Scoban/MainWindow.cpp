#include "MainWindow.h"
#include <time.h>

MainWindow* MainWindow::_Instance;
BitmapManager* MainWindow::Bit_Instance;
GameManager* MainWindow::Game_Instance;
bool MainWindow::isClear = false;
bool MainWindow::restart = false;
bool MainWindow::enemyInited = false;

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
		SetTimer(hwnd, 1, 10, nullptr);
		return 0;

	case WM_TIMER:
		if (isClear) {
			KillTimer(hwnd, 1);
			if (MessageBox(hwnd, L"Clear!", L"알림", MB_OK) == IDOK) {
				restart = true;
				enemyInited = false;
			}
		}
		if (Game_Instance->GetIsCollision()) {
			KillTimer(hwnd, 1);
			if (MessageBox(hwnd, L"Collision GameOver", L"알림", MB_OK) == IDOK) {
				restart = true;
				enemyInited = false;
			}
		}
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
		switch (wParam) {
		case 'R':
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
	srand(time(nullptr));
	InitBitManager();
	InitGameManager();
	Bit_Instance->LoadBitFile(m_hWnd);
	Game_Instance->LoadMap();
	Game_Instance->InitStage();
	m_hDC = GetDC(m_hWnd);
	isClear = false;
}

void MainWindow::OnUpdate() {
	if (!enemyInited) {
		Game_Instance->InitEnemy();
		enemyInited = true;
	}
	Game_Instance->EnemyMove();

	if (Game_Instance->CheckClear()) {
		OutputDebugString(L"Clear!");
		isClear = true;
	}
	if (restart) {
		Game_Instance->SetStage(Game_Instance->GetStage() + 1);
		Game_Instance->InitStage();
		isClear = false;
		restart = false;
		Game_Instance->SetIsCollision(false);
		SetTimer(m_hWnd, 1, 10, nullptr);
	}

	Game_Instance->Render(m_hDC);
}

void MainWindow::OnDestroy() {
	ReleaseDC(m_hWnd, m_hDC);
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
