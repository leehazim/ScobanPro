#include "MainWindow.h"
#include "BitmapManager.h"

MainWindow* MainWindow::_Instance = nullptr;
HWND MainWindow::Tiles[MAX_HEIGHT][MAX_WIDTH];
HDC MainWindow::m_ChildDC[MAX_HEIGHT][MAX_WIDTH];
LPCTSTR MainWindow::lpszClass = L"MainWindow";
LPCTSTR MainWindow::lpszChild = L"ChildWindow";
const int MainWindow::Max_stage = 10;
WCHAR* MainWindow::m_IdStage[10];


MainWindow::MainWindow() :
	countBox(0),
	countGoal(0),
	countStage(10),
	nowStage(0),
	selectTile(0)
{ 
	WCHAR stageName[100];
	for (int i = 0; i < Max_stage; i++) {
		wsprintf(stageName, L"stage%d", i + 1);
		wsprintf(m_IdStage[i], stageName);
	}
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
		BitmapManager::GetInstance()->LoadBitFile(m_hWnd);
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				Tiles[i][j] = CreateWindow(lpszChild, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
										   j * BLOCK_WIDTH, i * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT,
										   hwnd, (HMENU)nullptr, m_hInst, nullptr);
				SetWindowLongPtr(Tiles[i][j], 1, BitmapManager::tag_tile::WALL);
			}
		}
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

LRESULT MainWindow::ChildProc(HWND child, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_CREATE:
		SetWindowLongPtr(child, 1, 0);
		return 0;

	case WM_PAINT: {
		HDC hdc; PAINTSTRUCT ps;
		hdc = BeginPaint(child, &ps);
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				BitmapManager::GetInstance()->DrawBitmap(hdc, 0, 0, BitmapManager::GetInstance()->GetTile(GetWindowLongPtr(child, 1)));
			}
		}
		EndPaint(child, &ps);
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(child, Msg, wParam, lParam);
}

void MainWindow::Update() {

}

bool MainWindow::InitWindow(HINSTANCE hInstance, int nCmdShow) {
	m_hInst = hInstance;

	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.hInstance = hInstance;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildProc;
	WndClass.lpszClassName = lpszChild;
	WndClass.lpszMenuName = nullptr;
	WndClass.cbWndExtra = sizeof(int);
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

	m_MainDC = GetDC(m_hWnd);

	while (GetMessage(&Msg, nullptr, 0,0)) {
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

HWND MainWindow::GetTile(int x, int y) {
	return Tiles[y][x];
}

WCHAR* MainWindow::GetStageName(int idx) {
	return m_IdStage[idx];
}

const int MainWindow::GetMaxStage() {
	return Max_stage;
}
