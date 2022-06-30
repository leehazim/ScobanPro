#include "MainWindow.h"
#include "BitmapManager.h"
#include "FileMgr.h"

MainWindow* MainWindow::_Instance = nullptr;
HWND MainWindow::Tiles[MAX_HEIGHT][MAX_WIDTH] = {0};
HDC MainWindow::m_ChildDC[MAX_HEIGHT][MAX_WIDTH] = {0};
LPCTSTR MainWindow::lpszClass = L"MainWindow";
LPCTSTR MainWindow::lpszChild = L"ChildWindow";
const int MainWindow::Max_stage = 10;
WCHAR* MainWindow::m_IdStage[10];
tag_map MainWindow::Map[10] = {0};
int MainWindow::m_ClientWidth = 800;
int MainWindow::m_ClientHeight = 600;
HWND MainWindow::m_hStatic = nullptr;
HWND MainWindow::m_hListbox = nullptr;
int MainWindow::countBox= 0;
int MainWindow::countGoal= 0;
int MainWindow::countStage= 0;
int MainWindow::countEnemy= 0;
int MainWindow::nowStage= 0;
int MainWindow::selectTile= 0;


MainWindow::MainWindow() :
	m_MainDC(nullptr)
{ 
	WCHAR stageName[100];
	for (int i = 0; i < Max_stage; i++) {
		wsprintf(stageName, L"stage%d", i + 1);
		m_IdStage[i] = new WCHAR[lstrlen(stageName) + 1];
		lstrcpy(m_IdStage[i], stageName);
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
	{
		RECT rt = { 0,0, m_ClientWidth, m_ClientHeight };
		SetWindowPos(hwnd, NULL, 0, 0, rt.right, rt.bottom, SWP_NOMOVE);
		BitmapManager::GetInstance()->LoadBitFile(m_hWnd);
		MakeChild(hwnd);
	}
	return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case MESSAGE_CHANGE:
	{
		WCHAR str[100];
		int tmp;
		countGoal = 0;
		countBox = 0;
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				tmp = GetWindowLongPtr(Tiles[i][j], 1);
				if (tmp == BitmapManager::tag_tile::GOAL) countGoal++;
				else if (tmp == BitmapManager::tag_tile::BOX) countBox++;
			}
		}

		wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), countBox, countGoal);
		SetWindowText(m_hStatic, str);
	}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BTN_SAVE:
			FileMgr::Save();
			break;

		case ID_LOAD:
			FileMgr::Load();
			break;

		case ID_BTN_WALL:
		case ID_BTN_BOX:
		case ID_BTN_MAN:
		case ID_BTN_GOAL:
		case ID_BTN_WAY:
			selectTile = ChangeSelect(LOWORD(wParam));
			break;

		case ID_BTN_CLEAR:
			InitMap();
			break;

		case ID_LIST:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				for (int j = 0; j < MAX_HEIGHT; j++) {
					for (int k = 0; k < MAX_WIDTH; k++) {
						Map[nowStage].Map[j][k] = GetWindowLongPtr(Tiles[j][k], 1);
						Map[nowStage].cntBox = countBox;
						Map[nowStage].cntGoal = countGoal;
					}
				}
				nowStage = SendDlgItemMessage(hwnd, ID_LIST, LB_GETCURSEL, 0, 0);

				for (int i = 0; i < MAX_HEIGHT; i++) {
					for (int j = 0; j < MAX_WIDTH; j++) {
						SetWindowLongPtr(Tiles[i][j], 1, Map[nowStage].Map[i][j]);
						InvalidateRect(Tiles[i][j], NULL, TRUE);
					}
				}
				countBox = Map[nowStage].cntBox;
				countGoal = Map[nowStage].cntGoal;
				SendMessage(hwnd, MESSAGE_CHANGE, 0, 0);

				break;
			}
			break;
		}
		return 0;

	case WM_KEYDOWN:
	
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}
void MainWindow::InitMap() {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			SetWindowLongPtr(Tiles[i][j], 1, 0);
			InvalidateRect(Tiles[i][j], NULL, TRUE);
		}
	}
}

LRESULT MainWindow::ChildProc(HWND child, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_CREATE:
		SetWindowLongPtr(child, 1, 0);
		return 0;

	case WM_PAINT: {
		HDC hdc; PAINTSTRUCT ps;
		hdc = BeginPaint(child, &ps);
		BitmapManager::GetInstance()->DrawBitmap(hdc, 0, 0, BitmapManager::GetInstance()->GetTile(GetWindowLongPtr(child, 1)));
		
		EndPaint(child, &ps);
	}
	return 0;

	case WM_LBUTTONDOWN:
		SetWindowLongPtr(child, 1, selectTile);
		InvalidateRect(child, NULL, TRUE);

		SendMessage(m_hWnd, MESSAGE_CHANGE, 0, 0);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(child, Msg, wParam, lParam);
}

void MainWindow::MakeChild(HWND hwnd) {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			Tiles[i][j] = CreateWindow(lpszChild, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
									   j * BLOCK_WIDTH, i * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT,
									   hwnd, (HMENU)nullptr, m_hInst, nullptr);
			SetWindowLongPtr(Tiles[i][j], 1, BitmapManager::tag_tile::WALL);
		}
	}

	m_hStatic = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE,
						   10, MAX_HEIGHT * 32 + 10, 200, 25, hwnd, (HMENU)0, m_hInst, NULL);
	m_hListbox = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
							400, 10, 100, 300, hwnd, (HMENU)ID_LIST, m_hInst, NULL);
	for (int i = 0; i < Max_stage; i++) {
		SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)m_IdStage[i]);
	}
	SendMessage(m_hListbox, LB_SETCURSEL, 0, 0);

	WCHAR str[100];
	wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), MainWindow::GetSingleInstance()->GetMap(nowStage).cntBox, MainWindow::GetSingleInstance()->GetMap(nowStage).cntGoal);
	SetWindowText(m_hStatic, str);

	CreateWindow(L"button", L"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				 500, 300, 100, 25, hwnd, (HMENU)ID_BTN_CLEAR, m_hInst, NULL);
	CreateWindow(L"button", L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				 500, 330, 100, 25, hwnd, (HMENU)ID_BTN_SAVE, m_hInst, NULL);

	CreateWindow(L"button", L"WALL", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
				 400, 300, 100, 30, hwnd, (HMENU)ID_BTN_WALL, m_hInst, NULL);
	CreateWindow(L"button", L"BOX", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 330, 100, 30, hwnd, (HMENU)ID_BTN_BOX, m_hInst, NULL);
	CreateWindow(L"button", L"MAN", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 360, 100, 30, hwnd, (HMENU)ID_BTN_MAN, m_hInst, NULL);
	CreateWindow(L"button", L"GOAL", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 390, 100, 30, hwnd, (HMENU)ID_BTN_GOAL, m_hInst, NULL);
	CreateWindow(L"button", L"WAY", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 420, 100, 30, hwnd, (HMENU)ID_BTN_WAY, m_hInst, NULL);
	CheckRadioButton(hwnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WALL);
	selectTile = 0;
}

int MainWindow::ChangeSelect(WORD param) {
	int tmp = 0;
	switch (param) {
	case ID_BTN_WALL:
		tmp = 0;
		CheckRadioButton(m_hWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WALL);
		break;
	case ID_BTN_BOX:
		tmp = 1;
		CheckRadioButton(m_hWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_BOX);
		break;
	case ID_BTN_MAN:
		tmp = 2;
		CheckRadioButton(m_hWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_MAN);
		break;
	case ID_BTN_GOAL:
		tmp = 3;
		CheckRadioButton(m_hWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_GOAL);
		break;
	case ID_BTN_WAY:
		tmp = 4;
		CheckRadioButton(m_hWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WAY);
		break;
	}
	return tmp;
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

tag_map MainWindow::GetMap(int idx) {
	return Map[idx];
}
