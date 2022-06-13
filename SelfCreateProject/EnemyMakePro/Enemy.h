#pragma once
#include "Player.h"
#include "AutoMove.h"

class Enemy 
	: public AutoMove{
protected:
	int m_nStay, m_nFrame;
	Player* m_pPlayer;
	
public:
	Enemy(Player* pPlayer);

	bool GetExist() { return exist; }
	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
	bool IsCrash(Player* p);
	virtual void Draw(HDC);
	
	virtual void Move();
};

