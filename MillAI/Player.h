#pragma once

#include "Board.h"

class Player
{
public:
	virtual void turn() = 0;

	void setColor(int _color);
	int getColor();

	void setBoard(Board *_board);
protected:
	int m_color;
	Board *m_board;
};