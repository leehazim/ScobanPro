#include "01.Function.h"

/// <summary>
/// �ۼ��� : �̽���
/// �����ۼ��� : 2022. 05.14
/// ������������ : 2022.05.22
/// 
/// ��Ʈ�� ��θ� �����ͼ� DIB��Ʈ���� DDB��Ʈ������ ��ȯ
/// </summary>
/// <param name="hdc"> ��Ʈ���� �׸��� ���� DDB�� �ٲٱ� ���� �ӽ� HDC </param>
/// <param name="Path"> ��Ʈ������ ��� </param>
/// <returns> DDB�� ��ȯ�� ��Ʈ�� �ڵ� </returns>
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
/// �ۼ��� : �̽���
/// �����ۼ��� : 2022. 05.14
/// ������������ : 2022.05.22
/// 
/// ��Ʈ���� �׸��� �Լ� ����
/// </summary>
/// <param name="hdc"> ��Ʈ���� �׸� DC�ڵ� </param>
/// <param name="x"> ��Ʈ���� �׷��� x��ǥ </param>
/// <param name="y"> ��Ʈ���� �׷��� y��ǥ </param>
/// <param name="hBit"> �׸� ��Ʈ�� �ڵ� </param>
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