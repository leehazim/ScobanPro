#include "01.Function.h"
extern HWND g_hMainWnd;
extern int countStage;
extern const int Max_stage = 10;
extern HWND TileWnd[MAX_HEIGHT][MAX_WIDTH];
extern const TCHAR* ID_Stage[Max_stage];
extern tag_map Maps[Max_stage];
extern HBITMAP Tiles[5];
extern HWND hStatic, hListbox;
extern LPCTSTR lpszTile;
extern HINSTANCE g_hInst;
HWND TileTemp[150];

/// <summary>
/// 작성자 : 이승현
/// 최초작성일 : 2022. 05.14
/// 마지막수정일 : 2022.05.20
/// 
/// 비트맵 경로를 가져오는 공용 대화상자 함수에 포함
/// 비트맵 경로를 가져와서 DIB비트맵을 DDB비트맵으로 변환
/// </summary>
/// <param name="hdc"> 비트맵을 그리고 나서 DDB로 바꾸기 위한 임시 HDC </param>
/// <param name="Path"> 비트맵파일 경로 </param>
/// <returns> DDB로 변환된 비트맵 핸들 </returns>
void LoadMyBitmap(HWND hwnd) {
	for (int i = 0; i < 4; i++)
		Tiles[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
	Tiles[4] = nullptr;
	/*HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh;
	HBITMAP hBit;
	PVOID ih;

	OPENFILENAME OFN;
	TCHAR lpszStr[MAX_PATH] = TEXT("");
	HDC hdc;

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFile = lpszStr;
	OFN.lpstrFilter = TEXT("Bitmap File(*bmp)\0*.bmp\0");
	OFN.nMaxFile = MAX_PATH;

	for (int i = 0; i < COUNT_BITMAP; i++) {
		if (GetOpenFileName(&OFN) != 0) {
			hdc = GetDC(hwnd);

			hFile = CreateFile(lpszStr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				return;
			FileSize = GetFileSize(hFile, NULL);
			fh = (BITMAPFILEHEADER*)malloc(FileSize);
			ReadFile(hFile, fh, FileSize, &dwRead, NULL);
			CloseHandle(hFile);

			ih = ((PBYTE)fh + sizeof(BITMAPFILEHEADER));
			hBit = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)ih, CBM_INIT, (PBYTE)fh + fh->bfOffBits, (BITMAPINFO*)ih, DIB_RGB_COLORS);
			Tiles[i] = hBit;
			free(fh);
			ReleaseDC(hwnd, hdc);
		}
	}*/
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
/// 최종 수정일 : 2022.05.19
/// 
/// 현재 설정된 TileWnd들의 정보를 불러와서 구조체에 담는다.
/// 그리고 파일로 저장한다.
/// 
/// </summary>
/// <param name="temp"></param>
bool Save() {
	tag_map tmp;
	HANDLE hFile;
	DWORD dwWrite;
	TCHAR fileName[100];

	tmp.cntBox = 0;
	tmp.cntGoal = 0;
	tmp.stage = SendDlgItemMessage(g_hMainWnd, ID_LIST, LB_GETCURSEL, 0, 0) + 1;
	int playerExist = 0;
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			tmp.map[i][j] = GetWindowLongPtr(TileWnd[i][j], 0);
			if (GetWindowLongPtr(TileWnd[i][j], 0) == BOX) tmp.cntBox++;
			if (GetWindowLongPtr(TileWnd[i][j], 0) == GOAL) tmp.cntGoal++;
			if (GetWindowLongPtr(TileWnd[i][j], 0) == MAN) playerExist++;
		}
	}
	 
	if (playerExist != 1) {
		MessageBox(g_hMainWnd, L"플레이어는 1개 존재해야합니다.", L"알림", MB_OK);
		return false;
	}
	if (tmp.cntBox != tmp.cntGoal) {
		MessageBox(g_hMainWnd, L"목적지와 박스의 개수가 맞지 않습니다.", L"다시 시도!", MB_OK);
		return false;
	}

	wsprintf(fileName, L"C:\\Asset\\%s", ID_Stage[tmp.stage - 1]);
	hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(g_hMainWnd, TEXT("파일 생성 실패"), TEXT("알림"), MB_OK);
		return false;
	}
	WriteFile(hFile, &tmp, sizeof(tag_map), &dwWrite, NULL);
	CloseHandle(hFile);
	return true;
}

/// <summary>
/// 작성자 : 이승현
/// 최초 작성일 : 2022년 05월 18일
/// 최종 수정일 : 2022년 05월 19일
/// 
/// 내용:
/// 저장되어있던 맵파일들을 불러와서 메모리에 장착
/// </summary>
void Load() {
	tag_map tmp[Max_stage];
	tag_map* sub;
	HANDLE hFile;
	DWORD dwRead;
	TCHAR fileName[100];

	for(int i = 0; i < Max_stage; i++){
		wsprintf(fileName, L"C:\\Asset\\%s", ID_Stage[i]);
		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			sub = (tag_map*)calloc(1, sizeof(tag_map));
			tmp[i] = *sub;
			free(sub);
		}
		ReadFile(hFile, &tmp[i], sizeof(tag_map), &dwRead, NULL);
		CloseHandle(hFile);
	}

	for (int i = 0; i < Max_stage; i++) {
		Maps[i] = tmp[i];
	}
}

void CreateChild(HWND hwnd) {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			TileWnd[i][j] = CreateWindow(lpszTile, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
										 j * 32, i * 32, 32, 32, hwnd, (HMENU)NULL, g_hInst, NULL);
		}
	}

	hStatic = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE,
						   10, MAX_HEIGHT * 32 + 10, 200, 25, hwnd, (HMENU)0, g_hInst, NULL);
	hListbox = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
							400, 10, 100, 300, hwnd, (HMENU)ID_LIST, g_hInst, NULL);

	CreateWindow(L"button", L"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				 500, 400, 100, 25, hwnd, (HMENU)ID_BTN_CLEAR, g_hInst, NULL);
	CreateWindow(L"button", L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				 610, 400, 100, 25, hwnd, (HMENU)ID_BTN_SAVE, g_hInst, NULL);
}

int CreateSelect(HWND hwnd) {
	CreateWindow(L"button", L"WALL", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
				 400, 400, 100, 30, hwnd, (HMENU)ID_BTN_WALL, g_hInst, NULL);
	CreateWindow(L"button", L"BOX", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 430, 100, 30, hwnd, (HMENU)ID_BTN_BOX, g_hInst, NULL);
	CreateWindow(L"button", L"MAN", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 460, 100, 30, hwnd, (HMENU)ID_BTN_MAN, g_hInst, NULL);
	CreateWindow(L"button", L"GOAL", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 490, 100, 30, hwnd, (HMENU)ID_BTN_GOAL, g_hInst, NULL);
	CreateWindow(L"button", L"WAY", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
				 400, 520, 100, 30, hwnd, (HMENU)ID_BTN_WAY, g_hInst, NULL);
	CheckRadioButton(hwnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WALL);
	return 0;
}

int ChangeSelect(WORD param) {
	int tmp = 0;
	switch (param) {
	case ID_BTN_WALL:
		tmp = 0;
		CheckRadioButton(g_hMainWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WALL);
		break;
	case ID_BTN_BOX:
		tmp = 1;
		CheckRadioButton(g_hMainWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_BOX);
		break;
	case ID_BTN_MAN:
		tmp = 2;
		CheckRadioButton(g_hMainWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_MAN);
		break;
	case ID_BTN_GOAL:
		tmp = 3;
		CheckRadioButton(g_hMainWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_GOAL);
		break;
	case ID_BTN_WAY:
		tmp = 4;
		CheckRadioButton(g_hMainWnd, ID_BTN_WALL, ID_BTN_WAY, ID_BTN_WAY);
		break;
	}
	return tmp;
}

void InitMap() {
	
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			SetWindowLongPtr(TileWnd[i][j], 0, 0);
			InvalidateRect(TileWnd[i][j], NULL, TRUE);
		}
	}
}