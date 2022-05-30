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
	for (int i = 0; i < Max_Height; i++) {
		for (int j = 0; j < Max_Width; j++) {
			MemMap[i][j] = Map[nowStage][i][j];
			if (Map[nowStage][i][j] == BitDrawer->tag_tile::MAN) {
				px = j;
				py = i;
			}
		}
	}
}

void GameManager::Move(int Key) {
	int dx = 0, dy = 0;

	switch (Key) {
	case VK_LEFT: dx = -1; break;
	case VK_RIGHT: dx = +1; break;
	case VK_UP: dy = -1; break;
	case VK_DOWN: dy = +1; break;
	}

	if (MemMap[py + dy][px + dx] != BitmapManager::WALL) { /* 벽이 아닌 경우*/
		if (MemMap[py + dy][px + dx] == BitmapManager::BOX) {
			if (MemMap[py + (dy * 2)][px + (dx * 2)] != BitmapManager::WALL ||
				MemMap[py + (dy * 2)][px + (dx * 2)] != BitmapManager::BOX) {
				if (Map[nowStage][py][px] == BitmapManager::GOAL) {
					MemMap[py + dy][px + dx] = BitmapManager::GOAL;
				}
				else {
					MemMap[py + dy][px + dx] = BitmapManager::WAY;
				}
				MemMap[py + (dy * 2)][px + (dx * 2)] = BitmapManager::BOX;
			}
			else {
				return;
			}
		}
		px += dx;
		py += dy;
		InvalidateRect(MainWindow::GetSingleInstance()->GetHandleWnd(), NULL, TRUE);
	}
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
			BitDrawer->DrawBitmap(MemDC, j * 32, i * 32, BitDrawer->m_Tile[MemMap[i][j]]);
		}
	}
	
	BitBlt(hdc, 0, 0, rt.right - rt.left, rt.bottom - rt.top, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
