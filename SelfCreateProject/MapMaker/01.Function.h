#pragma once
#include <Windows.h>
#include <vector>
#include "resource1.h"

struct tag_map {
	int map[MAX_HEIGHT][MAX_WIDTH];
	int stage;
};

enum tag_Tile { WALL, BOX, MAN, GOAL, WAY };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TileProc(HWND, UINT, WPARAM, LPARAM);

HBITMAP LoadMyBitmap(HDC hdc, TCHAR* Path);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
bool Save();
void Load();