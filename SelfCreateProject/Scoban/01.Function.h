#pragma once
#include <Windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

enum tag_tile { WALL, BOX, MAN, GOAL, WAY };
struct tag_map {
	int Map[MAX_HEIGHT][MAX_WIDTH];
	int stage;
	int cntBox, cntGoal;
};

void InitWindow();

/* ���� ���� �Լ�*/
bool IsClear();
void Move(HDC hdc, int key);
void ResetGame();
void LoadMap();

/* ��Ʈ�� ���� �Լ�*/
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void InitBitmap();
void DestroyBitmap();
void Render(HDC hdc);