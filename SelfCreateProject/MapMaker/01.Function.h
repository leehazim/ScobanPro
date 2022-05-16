#pragma once
#include <Windows.h>

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32
#define MAX_WIDTH 10
#define MAX_HEIGHT 15
#define MAX_TILE 150
#define COUNT_BITMAP 4

struct tag_map {
	int map[MAX_HEIGHT][MAX_WIDTH];
	int stage;
};
enum tag_Tile { WALL, BOX, MAN, GOAL, WAY };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TileProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP LoadMyBitmap(HDC hdc, TCHAR* Path);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);