#pragma once
#include <vector>
#include "BitmapManager.h"

class GameManager {
private:
	static const int Max_Width = 10;
	static const int Max_Height = 15;
	static const int Max_Stage = 10;
	struct tag_Map {
		int Map[Max_Height][Max_Width];
		int stage;
		int cntBox, cntGoal;
	};
	/*int Map[Max_Stage][Max_Height][Max_Width];*/
	std::vector<tag_Map> Maps;
	std::vector<tag_Map>::iterator it;
	HBITMAP hBit;

	int MemMap[Max_Height][Max_Width];
	int nowStage;
	int px, py;

	BitmapManager* BitDrawer;

public:
	GameManager();
	~GameManager();
	
	int& GetStage();
	void InitStage();
	void Move(int Key);
	bool CheckClear();
	void Render(HDC hdc);
	void LoadMap();
};

