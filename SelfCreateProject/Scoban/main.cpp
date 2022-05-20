#include "01.Function.h"

/* ������� ������*/
LPCTSTR lpszClass = TEXT("Scoban");
HINSTANCE g_hInst;
HWND g_hWnd;

/* ���� ������ ������*/
HBITMAP hBitObject[5];
HBITMAP hBit;
const int Max_stage = 10;
int px = -1, py = -1;
int NowStage = 0;

/* �� �÷��̿� �迭*/
int MemMap[MAX_HEIGHT][MAX_WIDTH];
/* �� ����� �迭*/
tag_map Map[Max_stage];
// int Map[Max_stage][MAX_HEIGHT][MAX_WIDTH] = {
//	/*{
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WAY,GOAL,GOAL,WAY,WAY,WALL,WALL,WALL},
//	{WALL,WALL,WAY,BOX,BOX,WAY,WAY,WALL,WALL,WALL},
//	{WALL,WALL,WAY,WAY,WAY,WAY,WAY,WALL,WALL,WALL},
//	{WALL,WALL,WAY,WAY,MAN,WAY,WAY,WALL,WALL,WALL},
//	{WALL,WALL,WAY,WAY,WAY,WAY,WAY,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
//	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
//	}*/
//};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hRrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

	HWND hWnd;
	WNDCLASS ws;
	MSG Message;
	g_hInst = hInstance;

	ws.style = CS_HREDRAW | CS_VREDRAW;
	ws.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	ws.hInstance = hInstance;
	ws.lpfnWndProc = WndProc;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ws.lpszClassName = lpszClass;
	ws.lpszMenuName = NULL;
	RegisterClass(&ws);

	DWORD style = WS_OVERLAPPEDWINDOW;
	style &= ~WS_THICKFRAME;
	hWnd = CreateWindow(lpszClass, lpszClass, style,
						CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	int x, y;
	BITMAP bit;
	HDC hdc; PAINTSTRUCT ps;

	switch (iMessage) {	
	case WM_CREATE:
		g_hWnd = hwnd;
		InitBitmap();
		InitWindow();
		LoadMap();
		return 0;

	case WM_TIMER:
		hdc = GetDC(hwnd);
		
		ReleaseDC(hwnd, hdc);
		if (IsClear() == true) {
			KillTimer(hwnd, ID_TIMER);
			SendMessage(hwnd, MESSAGE_RESTART, 0, 0);
		}
		return 0;

	case MESSAGE_RESTART:
		if (MessageBox(hwnd, TEXT("Ŭ����! �ٽý����ϰڽ��ϱ�?"), TEXT("�˸�"), MB_OKCANCEL) == IDOK) {
			NowStage++;
			SendMessage(hwnd, MESSAGE_START, 0, 0);
		}
		return 0;

	case MESSAGE_START:
		if (NowStage >= Max_stage) {
			if (MessageBox(hwnd, TEXT("��� ���������� Ŭ�����߽��ϴ�. ó������ �ٽ��غ��ðڽ��ϱ�?"), TEXT("�˸�"), MB_OKCANCEL) == IDOK)
				NowStage = 0;
			else
				return 0;
		}
		ResetGame();
		SetTimer(hwnd, ID_TIMER, 10, NULL);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
		hdc = GetDC(hwnd);
		switch (wParam) {
		case 'S': SendMessage(hwnd, MESSAGE_START, 0, 0); break;		  
		}
		Move(hdc, wParam);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Render(hdc);
		/*if(hBit) DrawBitmap(hdc, 0, 0, hBit);*/
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DestroyBitmap();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}