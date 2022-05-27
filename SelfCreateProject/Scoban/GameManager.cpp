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

}

bool GameManager::CheckClear() {
	return false;
}

void GameManager::Render(HDC hdc) {

}
