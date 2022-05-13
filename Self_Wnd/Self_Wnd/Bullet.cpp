#include "Bullet.h"

void Bullet::Make(int key, Player& P) {
	switch (key) {
	case VK_SPACE: 
		exist = true;
		m_x = P.GetX();
		m_y = P.GetY();
		break;
	}
	
	switch (P.GetDirect()) {
	case VK_LEFT: m_dx = -1; m_dy = 0; break;
	case VK_RIGHT: m_dx = +1; m_dy = 0; break;
	case VK_UP: m_dx = 0; m_dy = -1; break;
	case VK_DOWN: m_dx = 0; m_dy = +1; break;
	}
}

void Bullet::Move() {

	m_x += m_dx;
	m_y += m_dy;
	count++;

	if (count == m_range) {
		exist = false;
		count = 0;
	}
}

void Bullet::Draw(HDC hdc) {
	HBRUSH hBrush, OldBrush;
	HPEN hPen, OldPen;
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	hPen = CreatePen(PS_NULL, 0, NULL);
	OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	OldPen = (HPEN)SelectObject(hdc, hPen);
	Ellipse(hdc, m_x, m_y, m_x + 10, m_y + 10);
	DeleteObject(SelectObject(hdc, OldBrush));
	DeleteObject(SelectObject(hdc, OldPen));
}

void Bullet::IsCrash(Enemy* E) {
	int x_range = E->GetX() - m_x; x_range = (x_range > 0) ? x_range : -x_range;
	int y_range = E->GetY() - m_y; y_range = (y_range > 0) ? y_range : -y_range;
	if (x_range < 7 && y_range < 7) {
		this->exist = false;
		E->SetExist(false);
	}
}
