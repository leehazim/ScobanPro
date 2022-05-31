#include "GameManager.h"
#include "MainWindow.h"
const int GameManager::Max_Width;
const int GameManager::Max_Height;
const int GameManager::Max_Stage;

GameManager::GameManager()
	: px(-1), py(-1), nowStage(1), hBit(nullptr) {
	BitDrawer = MainWindow::GetSingleInstance()->GetBitmapManager();
}

GameManager::~GameManager() {
	delete BitDrawer;
}

void GameManager::InitStage() {
	it = Maps.begin();
	for (int i = 1; i < nowStage; i++) it++;

	for (int i = 0; i < Max_Height; i++) {
		for (int j = 0; j < Max_Width; j++) {
			MemMap[i][j] = it->Map[i][j];
			if (it->Map[i][j] == BitDrawer->tag_tile::MAN) {
				px = j;
				py = i;
			}
		}
	}
	InvalidateRect(MainWindow::GetSingleInstance()->GetHandleWnd(), NULL, TRUE);
}

void GameManager::Move(int Key) {
	int dx = 0, dy = 0;
	switch (Key) {
	case VK_LEFT: dx = -1; break;
	case VK_RIGHT: dx = +1; break;
	case VK_UP: dy = -1; break;
	case VK_DOWN: dy = +1; break;
	default: return;
	}

	it = Maps.begin();
	for (int i = 1; i < nowStage; i++) {
		it++;
	}

	if (MemMap[py + dy][px + dx] != BitmapManager::tag_tile::WALL) { /* 벽이 아닌 경우*/
		if (MemMap[py + dy][px + dx] == BitmapManager::tag_tile::WAY ||
			MemMap[py + dy][px + dx] == BitmapManager::tag_tile::GOAL) { /* 이동하려는 경로가 빈공간인 경우*/
			if (it->Map[py][px] == BitmapManager::tag_tile::GOAL)
				MemMap[py][px] = BitmapManager::tag_tile::GOAL;
			else
				MemMap[py][px] = BitmapManager::tag_tile::WAY;
			MemMap[py + dy][px + dx] = BitmapManager::tag_tile::MAN;
		}
		else { /* 이동하려는 경로에 박스가 있는 경우 */
			if (MemMap[py + (dy * 2)][px + (dx * 2)] == BitmapManager::tag_tile::WALL ||
				MemMap[py + (dy * 2)][px + (dx * 2)] == BitmapManager::tag_tile::BOX) {
				dx = dy = 0;
			}
			else if (MemMap[py + (dy * 2)][px + (dx * 2)] == BitmapManager::tag_tile::WAY ||
					 MemMap[py + (dy * 2)][px + (dx * 2)] == BitmapManager::tag_tile::GOAL) {
				if (it->Map[py][px] == BitmapManager::tag_tile::GOAL)
					MemMap[py][px] = BitmapManager::tag_tile::GOAL;
				else
					MemMap[py][px] = BitmapManager::tag_tile::WAY;

				MemMap[py + (dy * 2)][px + (dx * 2)] = BitmapManager::tag_tile::BOX;
				MemMap[py + dy][px + dx] = BitmapManager::tag_tile::MAN;
			}
		}
	}
	else { /* 이동하려는 곳이 벽인 경우 이동 불가*/
		dx = dy = 0;
	}
	px += dx;
	py += dy;
	InvalidateRect(MainWindow::GetSingleInstance()->GetHandleWnd(), NULL, FALSE);
}

bool GameManager::CheckClear() {
	return false;
}

void GameManager::Render(HDC hdc) {
	HDC MemDC;
	HBITMAP OldBitmap;
	RECT rt;
	BITMAP bit;
	
	GetClientRect(MainWindow::GetSingleInstance()->GetHandleWnd(), &rt);
	MemDC = CreateCompatibleDC(hdc);
	if(hBit == nullptr)
		hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	GetObject(hBit, sizeof(BITMAP), &bit);
	FillRect(MemDC, &rt, HBRUSH(COLOR_WINDOW + 1));

	for (int i = 0; i < Max_Height; i++) {
		for (int j = 0; j < Max_Width; j++) {
			BitDrawer->DrawBitmap(MemDC, j * 32, i * 32, BitDrawer->GetTile(MemMap[i][j]));
		}
	}
	
	BitBlt(hdc, 0, 0, rt.right - rt.left, rt.bottom - rt.top, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void GameManager::LoadMap() {
	HANDLE hFile;
	tag_Map tmp;
	DWORD dwRead;
	TCHAR path[128];

	for (int i = 1; i <= 10; i++) {
		wsprintf(path, L"stage%d", i);
		hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) MessageBox(MainWindow::GetSingleInstance()->GetHandleWnd(), L"파일 열기 실패", L"알림", MB_OK);
		ReadFile(hFile, &tmp, sizeof(tag_Map), &dwRead, NULL);
		CloseHandle(hFile);

		Maps.push_back(tmp);
	}
}

int& GameManager::GetStage() {
	return nowStage;
}
