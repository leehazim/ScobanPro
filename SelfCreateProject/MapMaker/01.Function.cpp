#include "01.Function.h"
extern HWND g_hMainWnd;
extern int countStage;
std::vector<tag_map> Maps;

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
	if (hFile == INVALID_HANDLE_VALUE) 
		return NULL;
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

/// <summary>
/// �ۼ��� : �̽���
/// ���� �ۼ��� : 2022.05.17
/// ���� ������ : 2022.05.17
/// 
/// ���� ������ TileWnd���� ������ �ҷ��ͼ� ����ü�� ��´�.
/// �׸��� ���Ϸ� �����Ѵ�.
/// 
/// </summary>
/// <param name="temp"></param>
bool Save(HWND* Table) {
	
	tag_map tmp;
	int emptyStage = 0;
	std::vector<tag_map>::iterator it;
	HANDLE hFile;
	DWORD dwWrite;
	TCHAR fileName[100];
	emptyStage = Maps.size() + 1;
	
	tmp.stage = emptyStage;
	for (int i = 0, count = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			tmp.map[i][j] = GetWindowLongPtr(Table[count], 0);
			count++;
		}
	}
	wsprintf(fileName, TEXT("Map%d.file"), emptyStage);
	hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		if (MessageBox(g_hMainWnd, TEXT("���� ���� ����"), TEXT("����"), MB_OKCANCEL) == MB_OK) {
			return false;
		}
		else {
			if (Save(Table) == true) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	WriteFile(hFile, &tmp, sizeof(tmp), &dwWrite, NULL);
	CloseHandle(hFile);
}

void Load() {
	tag_map tmp;
	HANDLE hFile;
	DWORD dwRead;
	TCHAR fileName[100];
	int i = 0;

	while (true) {
		wsprintf(fileName, TEXT("Map%d.file"), i + 1);
		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) 
			break;
		i++;
		ReadFile(hFile, &tmp, sizeof(tag_map), &dwRead, NULL);
		Maps.push_back(tmp);
	}
	countStage = i;
}