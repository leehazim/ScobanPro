#pragma once
#include <windows.h>
#include "resource.h"

class MainWindow;
class BitmapManager {
public:
	static const int Max_Cnt_Tile;

public:
	enum tag_tile { WALL, BOX, MAN, GOAL, ENEMY, WAY };
	HBITMAP m_Tile[5];
	BitmapManager();
	~BitmapManager();

	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
	void LoadBitFile(HWND hwnd);
	HBITMAP GetTile(int idx);

private:
	HBITMAP m_Buffer;
};