#include "Player.h"

void Player::setColor(int _color) {
	m_color = _color;
}

int Player::getColor() {
	return m_color;
}

void Player::setBoard(Board *_board) {
	m_board = _board;
}