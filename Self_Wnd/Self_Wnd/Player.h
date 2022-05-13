#pragma once
#include <windows.h>
#include <stdlib.h>
#include <Math.h>
#include <time.h>

class Player{
private:
	static int m_x, m_y;
	int direct;
	/*tag_shape m_shape;*/
public:
	Player();

	int GetX() const;
	int GetY() const;
	void Move(int key);

	void Draw(HDC);
	int GetDirect() const { return direct; }
	void SetDirect(int key) { direct = key; }
};

