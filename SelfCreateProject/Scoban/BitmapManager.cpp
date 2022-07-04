#include "BitmapManager.h"
#include "MainWindow.h"
const int BitmapManager::Max_Cnt_Tile = 5;

BitmapManager::BitmapManager() {
	
}

BitmapManager::~BitmapManager() {
	for(int i = 0; i<4; i++)
		DeleteObject(m_Tile[i]);
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
	for (int i = 0; i < Max_Cnt_Tile; i++) {
		WCHAR str[100];
		wsprintf(str, L"C:/Asset/Bitmap%i.bmp", i + 1);
		m_Tile[i] = (HBITMAP)LoadImage(MainWindow::GetSingleInstance()->GetInstance(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	m_Tile[WAY] = nullptr;
}

HBITMAP BitmapManager::GetTile(int idx) {
	return m_Tile[idx];
}
