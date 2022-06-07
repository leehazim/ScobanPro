#include "01.Function.h"
#pragma comment(lib, "Msimg32")

// 윈도우클래스를 설정하기 위한 식별자
LPCTSTR lpszClass = TEXT("MapMaker");
LPCTSTR lpszTile = TEXT("Tile");

// 윈도우인스턴스와 윈도우핸들 전역변수로 관리
HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
HINSTANCE g_hInst;
HWND g_hMainWnd;
HWND hStatic;
HWND hListbox;

// Child윈도우들의 정보들 담아두기위한 변수들
HBITMAP Tiles[5];
int countBox = 0;
int countGoal = 0;
int countStage = 0;
int nowStage = 0;
int selectTile;

const int Max_stage = 10;
tag_map Maps[Max_stage];
const TCHAR* ID_Stage[Max_stage] = {
	TEXT("stage1"),
	TEXT("stage2"),
	TEXT("stage3"),
	TEXT("stage4"),
	TEXT("stage5"),
	TEXT("stage6"),
	TEXT("stage7"),
	TEXT("stage8"),
	TEXT("stage9"),
	TEXT("stage10")
};

/// <summary>
/// 작성일 : 2022년 5월 11일 
/// 최종 수정일 : 2022년 5월 14일
/// 
/// 내용:
/// 윈도우 진입함수 메인윈도우클래스 등록과 생성, 블록들의 정보들과 출력을 위한 윈도우클래스 등록
/// 메시지 루프.
/// </summary>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	WNDCLASS ws;
	MSG Msg;

	ws.style = CS_VREDRAW | CS_HREDRAW;
	ws.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	ws.lpfnWndProc = WndProc;
	ws.hInstance = hInstance;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ws.lpszClassName = lpszClass;
	ws.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&ws);

	ws.lpfnWndProc = TileProc;
	ws.cbWndExtra = sizeof(int);
	ws.lpszClassName = lpszTile;
	RegisterClass(&ws);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT, 1024, 800,
						NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return (int)Msg.wParam;
}

/// <summary>
/// 작성일 : 2022년 5월 11일
/// 최종 수정일 : 2022년 5월 17일
/// 
/// 내용:
/// 메인 윈도우 프로시저 비트맵을 불러오고 타일윈도우들을 생성하고,
/// 박스와 목적지 개수를 표기해줄 스태틱윈도우 생성
/// 
/// 주의사항:
///  최초 실행시 비트맵파일을 벽, 박스, 플레이어, 목적지 순으로 불러오기
/// </summary>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc; PAINTSTRUCT ps;
	TCHAR str[128];
	int tmp;

	switch (iMessage) {
	case WM_CREATE:
		g_hMainWnd = hwnd;
		LoadMyBitmap(hwnd);
		CreateChild(hwnd);
		selectTile = CreateSelect(hwnd);
		for (int i = 0; i < Max_stage; i++) {
			SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)ID_Stage[i]);
		}
		Load();
		for (int j = 0; j < MAX_HEIGHT; j++) {
			for (int k = 0; k < MAX_WIDTH; k++) {
				SetWindowLongPtr(TileWnd[j][k], 0, Maps[nowStage].map[j][k]);
			}
		}
		wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), Maps[nowStage].cntBox, Maps[nowStage].cntGoal);
		SetWindowText(hStatic, str);
		
		SendMessage(hListbox, LB_SETCURSEL, 0, 0);
		
		return 0;

	case MESSAGE_CHANGE:
		countGoal = 0;
		countBox = 0;
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				tmp = GetWindowLongPtr(TileWnd[i][j], 0);
				if (tmp == GOAL) countGoal++;
				else if (tmp == BOX) countBox++;
			}
		}

		wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), countBox, countGoal);
		SetWindowText(hStatic, str);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SAVE:
			Save();
			break;

		case ID_LOAD:
			Load();
			break;

		case ID_BTN_WALL:
		case ID_BTN_BOX: 
		case ID_BTN_MAN:
		case ID_BTN_GOAL:
		case ID_BTN_WAY:
			selectTile = ChangeSelect(LOWORD(wParam));
			break;

		case ID_LIST:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				for (int j = 0; j < MAX_HEIGHT; j++) {
					for (int k = 0; k < MAX_WIDTH; k++) {
						Maps[nowStage].map[j][k] = GetWindowLongPtr(TileWnd[j][k], 0);
						Maps[nowStage].cntBox = countBox;
						Maps[nowStage].cntGoal = countGoal;
					}
				}
				nowStage = SendDlgItemMessage(hwnd, ID_LIST, LB_GETCURSEL, 0, 0);

				for (int i = 0; i < MAX_HEIGHT; i++) {
					for (int j = 0; j < MAX_WIDTH; j++) {
						SetWindowLongPtr(TileWnd[i][j], 0, Maps[nowStage].map[i][j]);
						InvalidateRect(TileWnd[i][j], NULL, TRUE);
					}
				}
				countBox = Maps[nowStage].cntBox;
				countGoal = Maps[nowStage].cntGoal;
				SendMessage(hwnd, MESSAGE_CHANGE, 0, 0);

				break;
			}
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

/// <summary>
/// 작성일 : 2022년 5월 11일
/// 최종 수정일 : 2022년 5월 14일
/// 
/// 내용:
/// 타일윈도우 프로시저, 좌클릭시 타일이 변경,
/// 현재 상태에 따라 바로 스태틱에 출력할 개수 변경
/// 
/// </summary>
LRESULT CALLBACK TileProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	int tmp;
	HDC hdc; PAINTSTRUCT ps;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		SetWindowLongPtr(hwnd, 0, WALL);
		return 0;

	case WM_LBUTTONDOWN:
		SetWindowLongPtr(hwnd, 0, selectTile);
		InvalidateRect(hwnd, NULL, TRUE);
		
		SendMessage(g_hMainWnd, MESSAGE_CHANGE, 0, 0);
		
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawBitmap(hdc, 0, 0, Tiles[GetWindowLongPtr(hwnd, 0)]);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}