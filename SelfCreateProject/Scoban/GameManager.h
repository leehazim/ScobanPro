#pragma once
#include <vector>
#include "BitmapManager.h"

struct Enemy {
	int ex, ey;
	int stay;
	int movecount;
	bool isExist;
};

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
	int MoveCnt;
	bool IsCollision;
	std::vector<Enemy*> enemies;

	BitmapManager* BitDrawer;

public:
	GameManager();
	~GameManager();
	
	bool SetStage(int stage);
	int& GetStage();
	void InitStage();
	void Move(int Key);
	bool CheckClear();
	void Render(HDC hdc);
	void LoadMap();
	void EnemyMove();
	void InitEnemy();
	void Collision();

	bool GetIsCollision() const { return IsCollision; }
	void SetIsCollision(bool b) { IsCollision = b; }
};

