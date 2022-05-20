#include "01.Function.h"

extern HWND g_hWnd;
extern HBITMAP hBit;
extern HBITMAP hBitObject[5];
extern HINSTANCE g_hInst;

extern const int Max_stage = 10;
extern int NowStage;
extern tag_map Map[Max_stage];
extern int MemMap[MAX_HEIGHT][MAX_WIDTH];
extern int px, py;

bool IsClear() {

	for (int i = 0; i < MAX_WIDTH; i++) {
		for (int j = 0; j < MAX_HEIGHT; j++) {
			if (Map[NowStage].Map[j][i] == GOAL && MemMap[j][i] != BOX)
				return false;
		}
	}
	return true;
}

void Move(HDC hdc, int key) {
	HDC MemDC;
	HBITMAP OldBitmap;

	if (px == -1 || py == -1) return;
	int dx = 0, dy = 0;
	switch (key) {
	case VK_LEFT: dx = -1;	break;
	case VK_RIGHT:dx = +1;	break;
	case VK_UP:	dy = -1;	break;
	case VK_DOWN: dy = +1;	break;
	}

	if (MemMap[py + dy][px + dx] != WALL) { /* 벽이 아닌 경우*/
		if (MemMap[py + dy][px + dx] == WAY || MemMap[py + dy][px + dx] == GOAL) { /* 이동하려는 경로가 빈공간인 경우*/
			if (Map[NowStage].Map[py][px] == GOAL)
				MemMap[py][px] = GOAL;
			else
				MemMap[py][px] = WAY;
			MemMap[py + dy][px + dx] = MAN;
		}
		else { /* 이동하려는 경로에 박스가 있는 경우 */
			if (MemMap[py + (dy * 2)][px + (dx * 2)] == WALL || MemMap[py + (dy * 2)][px + (dx * 2)] == BOX) {
				dx = dy = 0;
			}
			else if (MemMap[py + (dy * 2)][px + (dx * 2)] == WAY || MemMap[py + (dy * 2)][px + (dx * 2)] == GOAL) {
				if (Map[NowStage].Map[py][px] == GOAL)
					MemMap[py][px] = GOAL;
				else
					MemMap[py][px] = WAY;

				MemMap[py + (dy * 2)][px + (dx * 2)] = BOX;
				MemMap[py + dy][px + dx] = MAN;
			}
		}
	}
	else { /* 이동하려는 곳이 벽인 경우 이동 불가*/
		dx = dy = 0;
	}
	py += dy;
	px += dx;

	InvalidateRect(g_hWnd, NULL, FALSE);
}

void ResetGame() {
	for (int i = 0; i < MAX_WIDTH; i++) {
		for (int j = 0; j < MAX_HEIGHT; j++) {
			if (Map[NowStage].Map[j][i] == MAN) {
				px = i;
				py = j;
			}
			MemMap[j][i] = Map[NowStage].Map[j][i];
		}
	}
}

void LoadMap() {
	HANDLE hFile;
	DWORD dwRead;
	TCHAR FileName[100];

	for (int i = 0; i < Max_stage; i++) {
		wsprintf(FileName, TEXT("stage%d"), i+1);
		hFile = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("알림"), MB_OK);
		}
		ReadFile(hFile, &Map[i], sizeof(tag_map), &dwRead, NULL);
		CloseHandle(hFile);
	}
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
	for (int i = 0; i < 4; i++)  
		DeleteObject(hBitObject[i]);
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

	/* 더블버퍼링을 위해서 메모리DC에 먼저그린다음에 빠른 복사를 위한 코딩 구조*/
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
