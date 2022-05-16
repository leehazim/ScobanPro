#include <Windows.h>
#include <vector>
#include "resource.h"
#pragma comment(lib, "Msimg32")

HBITMAP Tiles[5];
HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
enum tag_Tile { WALL, BOX, MAN, GOAL, WAY };
int countBox = 0, countGoal = 0, countStage = 0;
std::vector<tag_map> Maps;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TileProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LPCTSTR lpszClass = TEXT("MapMaker");
LPCTSTR lpszTile = TEXT("Tile");
HINSTANCE g_hInst;
HWND g_hMainWnd;
HWND hStatic; TCHAR str[128];


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

HBITMAP LoadMyBitmap(HDC hdc, TCHAR* Path) {

	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh;
	HBITMAP hBit;
	PVOID ih;

	hFile = CreateFile(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return NULL;
	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih = ((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	hBit = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)ih, CBM_INIT, (PBYTE)fh + fh->bfOffBits, (BITMAPINFO*)ih, DIB_RGB_COLORS);
	free(fh);
	return hBit;
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {

	HDC MemDC;
	HBITMAP OldBitmap;
	BITMAP bit;
	int bx, by;
	
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	
	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void SaveFile() {
	HANDLE hFile;
	tag_map tmp;
	std::vector<tag_map>::iterator it;
	it = Maps.begin();
	
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			tmp.map[i][j] = GetWindowLongPtr(TileWnd[i][j], 0);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME OFN;
	TCHAR lpszStr[MAX_PATH] = TEXT("");
	DWORD dwRead;
	HDC hdc; PAINTSTRUCT ps;
	RECT staticRect;

	switch (iMessage) {
	case WM_CREATE:
		g_hMainWnd = hwnd;
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hwnd;
		OFN.lpstrFile = lpszStr;
		OFN.lpstrFilter = TEXT("Bitmap File(*bmp)\0*.bmp\0");
		OFN.nMaxFile = MAX_PATH;
		for (int i = 0; i < 4; i++) {
			if (GetOpenFileName(&OFN) != 0) {
				hdc = GetDC(hwnd);
				Tiles[i] = LoadMyBitmap(hdc, lpszStr);
				ReleaseDC(hwnd, hdc);
			}
		}
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for(int j = 0; j < MAX_WIDTH; j++)
				TileWnd[i][j] = CreateWindow(lpszTile, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
										  j*32, i*32, 32, 32, hwnd, (HMENU)NULL, g_hInst, NULL);
		}

		wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), countBox, countGoal);
		
		hStatic = CreateWindow(TEXT("static"), str, WS_CHILD | WS_VISIBLE,
							   500, 10, 200, 25, hwnd, (HMENU)0, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SAVE:
			break;
		case ID_LOAD:
			break;
		}
		return 0;

	case WM_SIZE:
		SendMessage(hwnd, MESSAGE_STATICPAINT, 0, 0);
		return 0;

	case MESSAGE_STATICPAINT:
		GetClientRect(hStatic, &staticRect);
		hdc = GetDC(hStatic);
		FillRect(hdc, &staticRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		TextOut(hdc, 5, 5, str, lstrlen(str));
		ReleaseDC(hStatic, hdc);
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

LRESULT CALLBACK TileProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc; PAINTSTRUCT ps;
	int tmp;

	wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), countBox, countGoal);
	switch (iMessage) {
	case WM_CREATE:
		SetWindowLongPtr(hwnd, 0, WALL);
		return 0;

	case WM_LBUTTONDOWN:
		tmp = GetWindowLongPtr(hwnd, 0);
		if (tmp == BOX) countBox--;
		else if (tmp == GOAL) countGoal--;
		tmp++;
		if (tmp == BOX) countBox++;
		else if (tmp == GOAL) countGoal++;
		else if (tmp >= WAY) tmp = WALL;
		SetWindowLongPtr(hwnd, 0, tmp);
		InvalidateRect(hwnd, NULL, TRUE);
		wsprintf(str, TEXT("박스 개수 = %d, 골개수 = %d"), countBox, countGoal);
		SendMessage(g_hMainWnd, MESSAGE_STATICPAINT, 0, 0);
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