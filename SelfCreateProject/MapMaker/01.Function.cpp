#include "01.Function.h"
extern HWND g_hMainWnd;
extern int countStage;
extern const int Max_stage = 10;
extern HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
extern const TCHAR* ID_Stage[Max_stage];
extern tag_map Maps[Max_stage];
HWND TileTemp[150];

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

/// <summary>
/// 작성자 : 이승현
/// 최초 작성일 : 2022.05.17
/// 최종 수정일 : 2022.05.17
/// 
/// 현재 설정된 TileWnd들의 정보를 불러와서 구조체에 담는다.
/// 그리고 파일로 저장한다.
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
			if (MessageBox(g_hMainWnd, TEXT("파일 생성 실패"), TEXT("오류"), MB_OKCANCEL) == IDOK) {
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
/// 작성자 : 이승현
/// 최초 작성일 : 2022년 05월 18일
/// 최종 수정일 : 2022년 05월 18일
/// 
/// 내용:
/// 저장되어있던 맵파일들을 불러와서 메모리에 장착
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