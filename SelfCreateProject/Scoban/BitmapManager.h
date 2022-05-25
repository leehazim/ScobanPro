#pragma once
#include <windows.h>
class MainWindow;
class BitmapManager {
public:
	static const int Max_Cnt_Tile;

public:
	BitmapManager();
	~BitmapManager();

	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
	void LoadBitFile(HWND hwnd);
	HBITMAP GetTile(int idx);

private:
	HBITMAP m_Tile[5];
	HBITMAP m_Buffer;
};