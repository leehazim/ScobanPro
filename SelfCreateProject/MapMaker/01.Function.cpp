#include "01.Function.h"
extern HWND g_hMainWnd;
extern int countStage;
extern const int Max_stage = 10;
extern HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
extern const TCHAR* ID_Stage[Max_stage];
extern tag_map Maps[Max_stage];
HWND TileTemp[150];

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
bool Save() {
	tag_map tmp[10];
	HANDLE hFile;
	DWORD dwWrite;
	TCHAR fileName[100];
	
	for (int k = 0; k < Max_stage; k++) {
		for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				tmp[k].stage = k + 1;
				tmp[k].map[i][j] = GetWindowLongPtr(TileWnd[i][j], 0);
			}
		}
	}

	for (int i = 0; i < Max_stage; i++) {
		wsprintf(fileName, ID_Stage[i]);
		hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			if (MessageBox(g_hMainWnd, TEXT("���� ���� ����"), TEXT("����"), MB_OKCANCEL) == IDOK) {
				if (Save() == true) return true;
				else return false;
			}
			else {
				return false;
			}
		}
		WriteFile(hFile, &tmp[i], sizeof(tag_map), &dwWrite, NULL);
		CloseHandle(hFile);
	}
}

/// <summary>
/// �ۼ��� : �̽���
/// ���� �ۼ��� : 2022�� 05�� 18��
/// ���� ������ : 2022�� 05�� 18��
/// 
/// ����:
/// ����Ǿ��ִ� �����ϵ��� �ҷ��ͼ� �޸𸮿� ����
/// </summary>
void Load() {
	tag_map tmp[Max_stage];
	HANDLE hFile;
	DWORD dwRead;
	TCHAR fileName[100];

	for(int i = 0; i < Max_stage; i++){
		wsprintf(fileName, ID_Stage[i]);
		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) 
			break;
		ReadFile(hFile, &tmp[i], sizeof(tag_map), &dwRead, NULL);
	}
	for (int i = 0; i < Max_stage; i++) {
		Maps[i] = tmp[i];
	}
}