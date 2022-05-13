#include "Enemy.h"

Enemy::Enemy(Player* pPlayer) : AutoMove(rand() % 780, rand() % 630) {
	m_pPlayer = pPlayer;
	m_nStay = m_nFrame = (rand() % 20) + 1;

	
	/*if (m_y - m_pPlayer->GetY() > 0) m_y = 630;
	else m_y = 0;*/
}

bool Enemy::IsCrash(Player* p) {
	int x_range = p->GetX() - m_x; x_range = (x_range > 0) ? x_range : -x_range;
	int y_range = p->GetY() - m_y; y_range = (y_range > 0) ? y_range : -y_range;
	if (x_range < 7 && y_range < 7) {
		this->exist = false;
		return true;
	}
	return false;
}

void Enemy::Move() {

	if (m_pPlayer->GetX() - m_x > 0)	m_dx = +1;
	else								m_dx = -1;

	if ((m_pPlayer->GetY() - m_y) > 0)	m_dy = +1;
	else								m_dy = -1;

	if (--m_nStay == 0) {
		m_nStay = m_nFrame;
		m_x += m_dx;
		m_y += m_dy;
	}
}

void Enemy::Draw(HDC hdc) {
	HBRUSH hBrush, OldBrush;
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, m_x, m_y, m_x + 10, m_y + 10);
	DeleteObject(SelectObject(hdc, OldBrush));
}
