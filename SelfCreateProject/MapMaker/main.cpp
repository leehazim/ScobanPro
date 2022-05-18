#include "01.Function.h"
#pragma comment(lib, "Msimg32")

// ������Ŭ������ �����ϱ� ���� �ĺ���
LPCTSTR lpszClass = TEXT("MapMaker");
LPCTSTR lpszTile = TEXT("Tile");

// �������ν��Ͻ��� �������ڵ� ���������� ����
HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
HINSTANCE g_hInst;
HWND g_hMainWnd;
HWND hStatic;
HWND hlistbox;

// Child��������� ������ ��Ƶα����� ������
HBITMAP Tiles[5];
int countBox = 0;
int countGoal = 0;
int countStage = 0;
int nowStage;

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
/// �ۼ��� : 2022�� 5�� 11�� 
/// ���� ������ : 2022�� 5�� 14��
/// 
/// ����:
/// ������ �����Լ� ����������Ŭ���� ��ϰ� ����, ��ϵ��� ������� ����� ���� ������Ŭ���� ���
/// �޽��� ����.
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
/// �ۼ��� : 2022�� 5�� 11��
/// ���� ������ : 2022�� 5�� 17��
/// 
/// ����:
/// ���� ������ ���ν��� ��Ʈ���� �ҷ����� Ÿ����������� �����ϰ�,
/// �ڽ��� ������ ������ ǥ������ ����ƽ������ ����
/// 
/// ���ǻ���:
///  ���� ����� ��Ʈ�������� ��, �ڽ�, �÷��̾�, ������ ������ �ҷ�����
/// </summary>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME OFN;
	TCHAR lpszStr[MAX_PATH] = TEXT("");
	DWORD dwRead;
	HDC hdc; PAINTSTRUCT ps;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		g_hMainWnd = hwnd;
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hwnd;
		OFN.lpstrFile = lpszStr;
		OFN.lpstrFilter = TEXT("Bitmap File(*bmp)\0*.bmp\0");
		OFN.nMaxFile = MAX_PATH;
		for (int i = 0; i < COUNT_BITMAP; i++) {
			if (GetOpenFileName(&OFN) != 0) {
				hdc = GetDC(hwnd);
				Tiles[i] = LoadMyBitmap(hdc, lpszStr);
				ReleaseDC(hwnd, hdc);
			}
		}
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				TileWnd[i][j] = CreateWindow(lpszTile, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
											 j * 32, i * 32, 32, 32, hwnd, (HMENU)NULL, g_hInst, NULL);
			}
		}


		wsprintf(str, TEXT("�ڽ� ���� = %d, �񰳼� = %d"), countBox, countGoal);
		hStatic = CreateWindow(TEXT("static"), str, WS_CHILD | WS_VISIBLE,
							   10, MAX_HEIGHT * 32 + 10, 200, 25, hwnd, (HMENU)0, g_hInst, NULL);
		hlistbox = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
								 400, 10, 100, 300, hwnd, (HMENU)ID_LIST, g_hInst, NULL);

	for (int i = 0; i < Max_stage; i++) {
			SendMessage(hlistbox, LB_ADDSTRING, 0, (LPARAM)ID_Stage[i]);
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SAVE:
			Save();
			break;

		case ID_LOAD:
			Load();
			break;

		case ID_LIST:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				for (int i = 0; i < Max_stage; i++) {
					GetWindowText(hlistbox, str, 100);
					if (lstrcmp(str, ID_Stage[i]) == 0) {
						nowStage = i;
					}
				}	

				for (int i = 0; i < MAX_HEIGHT; i++) {
					for (int j = 0; j < MAX_WIDTH; j++) {
						SetWindowLongPtr(TileWnd[i][j], 0, Maps[nowStage].map[i][j]);
						InvalidateRect(TileWnd[i][j], NULL, TRUE);
					}
				}
				
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
/// �ۼ��� : 2022�� 5�� 11��
/// ���� ������ : 2022�� 5�� 14��
/// 
/// ����:
/// Ÿ�������� ���ν���, ��Ŭ���� Ÿ���� ����,
/// ���� ���¿� ���� �ٷ� ����ƽ�� ����� ���� ����
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

	case MESSAGE_CHANGE:
		SetWindowLongPtr(hwnd, 0, lParam);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_LBUTTONDOWN:
		tmp = GetWindowLongPtr(hwnd, 0);
		tmp++;
		if (tmp > WAY) tmp = WALL;
		SetWindowLongPtr(hwnd, 0, tmp);
		InvalidateRect(hwnd, NULL, TRUE);
		
		countGoal = 0;
		countBox = 0;
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				tmp = GetWindowLongPtr(TileWnd[i][j], 0);
				if (tmp == GOAL) countGoal++;
				else if (tmp == BOX) countBox++;
			}
		}

		wsprintf(str, TEXT("�ڽ� ���� = %d, �񰳼� = %d"), countBox, countGoal);
		SetWindowText(hStatic, str);
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