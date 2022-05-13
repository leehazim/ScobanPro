#include "Player.h"

int Player::m_x;
int Player::m_y;

Player::Player() {
	m_x = 400;
	m_y = 340;
	direct = VK_LEFT;
	/*m_shape = RECTANGLE;*/
}

int Player::GetX() const {
	return m_x;
}

int Player::GetY() const{
	return m_y;
}

void Player::Move(int key){
	

	if (GetKeyState(VK_LEFT) & 0x8000) { m_x = max(m_x - 10, 10); direct = VK_LEFT; }
	if (GetKeyState(VK_RIGHT) & 0x8000) { m_x = min(m_x + 10, 700); direct = VK_RIGHT; }
	if (GetKeyState(VK_UP) & 0x8000) { m_y = max(m_y - 10, 10); direct = VK_UP; }
	if (GetKeyState(VK_DOWN) & 0x8000) { m_y = min(m_y + 10, 530); direct = VK_DOWN; }
	
	/*switch (key) {
	case VK_LEFT: m_x = max(m_x - 10, 10); direct = VK_LEFT; break;
	case VK_RIGHT: m_x = min(m_x + 10, 700); direct = VK_RIGHT; break;
	case VK_UP: m_y = max(m_y - 10, 10); direct = VK_UP; break;
	case VK_DOWN: m_y = min(m_y + 10, 530); direct = VK_DOWN; break;
	}*/
}

void Player::Draw(HDC hdc){

	Rectangle(hdc, m_x, m_y, m_x + 10, m_y + 10);
}
