#pragma once

#include <chrono>
#include <thread>
#include "Player.h"

using namespace std::chrono;


struct Move {
	Move(){
		fromX = fromY = toX = toY = killX = killY = value = -1;
	}
	Move(int _fromX, int _fromY, int _toX, int _toY, int _killX, int _killY, int _value){
		fromX = _fromX;
		fromY = _fromY;
		toX = _toX;
		toY = _toY;
		killX = _killX;
		killY = _killY;
		value = _value;
	}
	int fromX;
	int fromY;
	int toX;
	int toY;
	int killX = -1;
	int killY = -1;
	int value;
};

class Computer : public Player
{
public:
	Computer();
	~Computer();

	void calculate(State *_state);
	void turn();
private:
	std::vector<Move> placing(State *_state, int _nextPlayer);
	std::vector<Move> moving(State *_state, int _nextPlayer);
	std::vector<Move> jumping(State *_state, int _nextPlayer);
	int removeStoneVal(State &_state, int _depth, int _alpha, int _beta, int _player);
	Move removeStone(State *_state, int _depth, int _alpha, int _beta, int _player);
	int getStateValue(State &_state, int _player);
	int negamax(State &_state, int _depth, int _alpha, int _beta);
	std::vector<Move> getMaxMoves(State *_state, std::vector<Move> _moves);

};

