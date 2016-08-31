#pragma once

#include <vector>

#include "definitions.h"

struct Field{
	Field(int _x, int _y) {
		x = _x;
		y = _y;
		color = -1;
		left = right = up = down = NULL;
	}
	int color;
	int x;
	int y;
	Field *left;
	Field *right;
	Field *up;
	Field *down;
};

class State {
public:
	State();
	State(const State &_state);

	void set(int _x, int _y, int _color);
	Field* get(int _x, int _y);
	std::vector<std::vector<Field>>* getFields();

	int stoneAmount(int _color);
	bool partOfMill(int _x, int _y);
	int stonesMovable(int _color);
	bool stonesRemovable(int _color);
	int millAmount(int _color);

	void setPlayer(int _player);
	void setGameState(int _gameState);
	void setTurn(int _turn);

	int getPlayer();
	int getGameState();
	int getTurn();

	void millSet();
	int lastMill();

	void setBlocked(bool _blocked);
	bool isBlocked();

private:
	void fill(int a, int b, int c);

	std::vector<std::vector<Field>> m_fields;
	int m_currentPlayer;
	int m_currenGameState;
	int m_currentTurn;
	int m_lastMill;

	bool m_blocked;
};
