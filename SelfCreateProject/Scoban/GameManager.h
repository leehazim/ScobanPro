#pragma once
#include<windows.h>
#include "BitmapManager.h"

class MainWindow;
class GameManager {
private:
	static const int Max_Width = 10;
	static const int Max_Height = 15;
	static const int Max_Stage = 10;
	int Map[Max_Stage][Max_Height][Max_Width];
	HBITMAP hBit;

	int MemMap[Max_Height][Max_Width];
	int nowStage;
	int px, py;

	BitmapManager* BitDrawer;

public:
	GameManager();
	~GameManager();
	
	void InitStage();
	void Move(int Key);
	bool CheckClear();
	void Render(HDC hdc);
};

