#pragma once
#include <windows.h>
class BitmapManager {
private:
	static const int Max_Cnt_Tile;

	HBITMAP* m_Tile;
	HBITMAP m_Buffer;
	
public:
	BitmapManager();
	~BitmapManager();
	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
	void LoadBitFile(HWND hwnd);
};