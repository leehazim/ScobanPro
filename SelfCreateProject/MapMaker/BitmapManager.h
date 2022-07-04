#pragma once
#include <windows.h>
#include "resource1.h"

class MainWindow;
class BitmapManager {
public:
	static const int Max_Cnt_Tile;

public:
	static enum tag_tile { WALL, BOX, MAN, GOAL, ENEMY, WAY };
	static HBITMAP m_Tile[6];
	BitmapManager();
	~BitmapManager();

	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
	void LoadBitFile(HWND hwnd);
	HBITMAP GetTile(int idx);

	static BitmapManager* GetInstance();
	
private:
	static BitmapManager* _Instance;
	static int countResource;
	HBITMAP m_Buffer;
};