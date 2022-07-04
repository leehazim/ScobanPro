#include "BitmapManager.h"
#include "MainWindow.h"

const int BitmapManager::Max_Cnt_Tile = 5;
HBITMAP BitmapManager::m_Tile[6];
BitmapManager* BitmapManager::_Instance = nullptr;
enum BitmapManager::tag_tile;
int BitmapManager::countResource = 5;

BitmapManager::BitmapManager() {
	
}

BitmapManager::~BitmapManager() {
	for (int i = 0; i < countResource; i++) DeleteObject(m_Tile[i]);
	delete _Instance;
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
	for (int i = 0; i < countResource; i++) {
	
		/*m_Tile[i] = LoadBitmap(MainWindow::GetSingleInstance()->GetInstance(), MAKEINTRESOURCE(IDB_BITMAP1 + i));*/
		WCHAR str[100];
		wsprintf(str, L"C:/Asset/Bitmap%d.bmp", i+1);
		m_Tile[i] = (HBITMAP)LoadImage(MainWindow::GetSingleInstance()->GetInstance(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	m_Tile[WAY] = nullptr;
}

HBITMAP BitmapManager::GetTile(int idx) {
	if(idx >= 0 || idx < countResource)
		return m_Tile[idx];
	return m_Tile[WAY];
}

BitmapManager* BitmapManager::GetInstance() {
	if (_Instance == nullptr) {
		_Instance = new BitmapManager();
	}
	return _Instance;
}
