#include <Windows.h>
#include "resource.h"
/* ������� ������*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR lpszClass = TEXT("Scoban");
HINSTANCE g_hInst;
HWND g_hWnd;

/* ���� ������ ������*/
enum tag_tile{ WALL, BOX, MAN, GOAL, WAY };
HBITMAP hBitObject[5];
HBITMAP hBit;
const int stage = 3;
int px = -1, py = -1;
int NowStage = 0;

/* �� �÷��̿� �迭*/
int MemMap[MAX_HEIGHT][MAX_WIDTH];
/* �� ����� �迭*/
 tag_tile Map[stage][MAX_HEIGHT][MAX_WIDTH] = {
	{
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WAY,GOAL,GOAL,WAY,WAY,WALL,WALL,WALL},
	{WALL,WALL,WAY,BOX,BOX,WAY,WAY,WALL,WALL,WALL},
	{WALL,WALL,WAY,WAY,WAY,WAY,WAY,WALL,WALL,WALL},
	{WALL,WALL,WAY,WAY,MAN,WAY,WAY,WALL,WALL,WALL},
	{WALL,WALL,WAY,WAY,WAY,WAY,WAY,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
	{WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
	}
};

 void InitWindow();

/* ��Ʈ�� ���� �Լ�*/
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void InitBitmap();
void DestroyBitmap();
void Render(HDC hdc);


void ResetGame();
void Move(HDC hdc,int key);
bool IsClear();


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
		if (MessageBox(hwnd, TEXT("Ŭ����! �ٽý����ϰڽ��ϱ�?"), TEXT("�˸�"), MB_OKCANCEL) == IDOK)
			SendMessage(hwnd, MESSAGE_START, 0, 0);
		return 0;

	case MESSAGE_START:
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

void InitWindow() {
	int x, y;
	BITMAP bit;

	GetObject(hBitObject[0], sizeof(BITMAP), &bit);
	x = bit.bmWidth;
	y = bit.bmHeight;
	SetWindowPos(g_hWnd, NULL, 0, 0, x * MAX_WIDTH * 2, (y + 3) * MAX_HEIGHT, SWP_NOMOVE);
}

void InitBitmap() {
	hBitObject[WALL] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL));
	hBitObject[BOX] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOX));
	hBitObject[MAN] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MAN));
	hBitObject[GOAL] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GOAL));
	hBitObject[WAY] = NULL;
}

void DestroyBitmap() {
	for (int i = 0; i < 4; i++)  DeleteObject(hBitObject[i]);
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP OldBitmap;
	BITMAP bit;
	int bx, by;
	
	if (hBit == NULL) return;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);
	
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void Render(HDC hdc) {
	HDC MemDC;
	HBITMAP OldBitmap;
	RECT rt;

	/* ������۸��� ���ؼ� �޸�DC�� �����׸������� ���� ���縦 ���� �ڵ� ����*/
	MemDC = CreateCompatibleDC(hdc);
	GetClientRect(g_hWnd, &rt);
	if (hBit == NULL) hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	FillRect(MemDC, &rt, (HBRUSH)(COLOR_WINDOW + 1));
	for (int i = 0; i < MAX_HEIGHT; i++)
		for (int j = 0; j < MAX_WIDTH; j++)
			DrawBitmap(MemDC, j * 32, i * 32, hBitObject[MemMap[i][j]]);
	
	BitBlt(hdc, 0, 0, rt.right - rt.left, rt.bottom - rt.top, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void ResetGame() {
	for (int i = 0; i < MAX_WIDTH; i++) {
		for (int j = 0; j < MAX_HEIGHT; j++) {
			if (Map[NowStage][j][i] == MAN) {
				px = i;
				py = j;
			}
			MemMap[j][i] = Map[0][j][i];
		}
	}
}

void Move(HDC hdc, int key) {
	HDC MemDC;
	HBITMAP OldBitmap;
	RECT rt;

	if (px == -1 || py == -1) return;
	int dx = 0, dy = 0;
	switch (key) {
	case VK_LEFT: dx = -1;	break;
	case VK_RIGHT:dx = +1;	break;
	case VK_UP:	dy = -1;	break;
	case VK_DOWN: dy = +1;	break;
	}

	if (MemMap[py + dy][px + dx] != WALL) { /* ���� �ƴ� ���*/
		if (MemMap[py + dy][px + dx] == WAY || MemMap[py + dy][px + dx] == GOAL) { /* �̵��Ϸ��� ��ΰ� ������� ���*/
			if (Map[NowStage][py][px] == GOAL)
				MemMap[py][px] = GOAL;
			else	
				MemMap[py][px] = WAY;
			MemMap[py + dy][px + dx] = MAN;
		}
		else { /* �̵��Ϸ��� ��ο� �ڽ��� �ִ� ��� */
			if (MemMap[py + (dy * 2)][px + (dx * 2)] ==  WALL || MemMap[py + (dy * 2)][px + (dx * 2)] == BOX) {
				dx = dy = 0;
			}
			else if (MemMap[py + (dy * 2)][px + (dx * 2)] == WAY || MemMap[py + (dy * 2)][px + (dx * 2)] == GOAL) {
				if (Map[NowStage][py][px] == GOAL)
					MemMap[py][px] = GOAL;
				else 
					MemMap[py][px] = WAY;

				MemMap[py + (dy * 2)][px + (dx * 2)] = BOX;
				MemMap[py + dy][px + dx] = MAN;
			}
		}
	}
	else { /* �̵��Ϸ��� ���� ���� ��� �̵� �Ұ�*/
		dx = dy = 0;
	}
	py += dy;
	px += dx;

	/*MemDC = CreateCompatibleDC(hdc);
	GetClientRect(g_hWnd, &rt);
	if (hBit == NULL) hBit = CreateCompatibleBitmap(MemDC, rt.right, rt.bottom);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			DrawBitmap(MemDC, j * 32, i * 32, hBitObject[MemMap[i][j]]);
		}
	}
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);*/

	InvalidateRect(g_hWnd, NULL, FALSE);
}

bool IsClear() {
	
	for (int i = 0; i < MAX_WIDTH; i++) {
		for (int j = 0; j < MAX_HEIGHT; j++) {
			if (Map[NowStage][j][i] == GOAL && MemMap[j][i] != BOX)
				return false;
		}
	}
	return true;
}