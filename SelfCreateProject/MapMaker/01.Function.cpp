#include "01.Function.h"

/// <summary>
/// 작성자 : 이승현
/// 최초작성일 : 2022. 05.14
/// 마지막수정일 : 2022.05.22
/// 
/// 비트맵 경로를 가져와서 DIB비트맵을 DDB비트맵으로 변환
/// </summary>
/// <param name="hdc"> 비트맵을 그리고 나서 DDB로 바꾸기 위한 임시 HDC </param>
/// <param name="Path"> 비트맵파일 경로 </param>
/// <returns> DDB로 변환된 비트맵 핸들 </returns>
HBITMAP LoadMyBitmap(HDC hdc, TCHAR* Path) {

	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh;
	HBITMAP hBit;
	PVOID ih;

	hFile = CreateFile(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return NULL;
	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih = ((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	hBit = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)ih, CBM_INIT, (PBYTE)fh + fh->bfOffBits, (BITMAPINFO*)ih, DIB_RGB_COLORS);
	free(fh);
	return hBit;
}

/// <summary>
/// 작성자 : 이승현
/// 최초작성일 : 2022. 05.14
/// 마지막수정일 : 2022.05.22
/// 
/// 비트맵을 그리는 함수 랩핑
/// </summary>
/// <param name="hdc"> 비트맵을 그릴 DC핸들 </param>
/// <param name="x"> 비트맵이 그려질 x좌표 </param>
/// <param name="y"> 비트맵이 그려질 y좌표 </param>
/// <param name="hBit"> 그릴 비트맵 핸들 </param>
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {

	HDC MemDC;
	HBITMAP OldBitmap;
	BITMAP bit;
	int bx, by;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}