#include "BitmapManager.h"

const int BitmapManager::Max_Cnt_Tile = 5;

BitmapManager::BitmapManager() {
	m_Tile = new HBITMAP[5];
}

BitmapManager::~BitmapManager() {
	delete[] m_Tile;
}

void BitmapManager::DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	BITMAP bit;
	GetObject(hBit, sizeof(BITMAP), &bit);
	HDC MemDC = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	BitBlt(hdc, x, y, bit.bmWidth, bit.bmHeight, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void BitmapManager::LoadBitFile(HWND hwnd) {
	HANDLE hFile;
	DWORD dwRead, FileSize;
	TCHAR str[MAX_PATH];
	BITMAPFILEHEADER* fh;
	PVOID ih;
	HDC hdc;

	for (int i = 0; i < Max_Cnt_Tile - 1; i++) {
		hdc = GetDC(hwnd);
		wsprintf(str, L"C:\\Users\\User\\Documents\\GitHub\\ScobanPro\\SelfCreateProject\\Asset\\Bitmap%d", i + 1);
		hFile = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) MessageBox(hwnd, L"파일 열기 실패", L"오류", MB_OK);
		FileSize = GetFileSize(hFile, NULL);
		fh = (BITMAPFILEHEADER*)malloc(FileSize);
		ReadFile(hFile, fh, FileSize, &dwRead, NULL);
		CloseHandle(hFile);

		ih = ((PBYTE)fh + sizeof(BITMAPFILEHEADER));
		m_Buffer = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)ih, CBM_INIT, (PBYTE)fh + fh->bfOffBits, (BITMAPINFO*)ih, DIB_RGB_COLORS);
		m_Tile[i] = m_Buffer;
		free(fh);
		ReleaseDC(hwnd, hdc);
	}
}
