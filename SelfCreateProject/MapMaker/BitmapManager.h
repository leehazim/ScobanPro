#pragma once
#include <windows.h>
#include "resource1.h"

class MainWindow;
class BitmapManager {
public:
	static const int Max_Cnt_Tile;

public:
	enum tag_tile { WALL, BOX, MAN, GOAL, WAY };
	HBITMAP m_Tile[5];
	BitmapManager();
	~BitmapManager();

	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
	void LoadBitFile(HWND hwnd);
	HBITMAP GetTile(int idx);
	static BitmapManager* GetInstance();
	
private:
	static BitmapManager* _Instance;
	HBITMAP m_Buffer;
};