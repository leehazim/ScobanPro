#include "BitmapManager.h"
#include "MainWindow.h"
#include "resource.h"
const int BitmapManager::Max_Cnt_Tile = 5;

BitmapManager::BitmapManager() {
	
}

BitmapManager::~BitmapManager() {

}

void BitmapManager::DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	BITMAP bit;
	if (hBit == nullptr) return;
	GetObject(hBit, sizeof(BITMAP), &bit);
	HDC MemDC = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	BitBlt(hdc, x, y, bit.bmWidth, bit.bmHeight, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void BitmapManager::LoadBitFile(HWND hwnd) {
	for (int i = 0; i < Max_Cnt_Tile - 1; i++) 
		m_Tile[i] = LoadBitmap(MainWindow::GetSingleInstance()->GetInstance(),
							   MAKEINTRESOURCE(IDB_BITMAP1+i));
	m_Tile[4] = nullptr;
}

HBITMAP BitmapManager::GetTile(int idx) {
	return m_Tile[idx];
}
