#include "State.h"



#include <stddef.h>
#include "State.h"
#include "definitions.h"

/*
	Initialises the fields and sets their neighbours
*/
void State::fill(int a, int b, int c){

	// Fill with color
	m_fields[a][a].color = m_fields[b][a].color = m_fields[c][a].color = COLOR_NONE;
	m_fields[a][c].color = m_fields[b][c].color = m_fields[c][c].color = COLOR_NONE;

	// Upper Left
	m_fields[a][a].right = &m_fields[b][a];
	m_fields[a][a].down = &m_fields[a][b];

	// Upper Right
	m_fields[c][a].left = &m_fields[b][a];
	m_fields[c][a].down = &m_fields[c][b];

	// Lower Left
	m_fields[a][c].right = &m_fields[b][c];
	m_fields[a][c].up = &m_fields[a][b];

	// Lower Right
	m_fields[c][c].up = &m_fields[c][b];
	m_fields[c][c].left = &m_fields[b][c];

	// Upper Middle
	if (a > 0) m_fields[b][a].up = &m_fields[b][a - 1];
	if (a < 2) m_fields[b][a].down = &m_fields[b][a + 1];
	m_fields[b][a].left = &m_fields[a][a];
	m_fields[b][a].right = &m_fields[c][a];

	// Left Middle
	m_fields[a][b].up = &m_fields[a][a];
	m_fields[a][b].down = &m_fields[a][c];
	if (a > 0) m_fields[a][b].left = &m_fields[a - 1][b];
	if (a < 2) m_fields[a][b].right = &m_fields[a + 1][b];

	// Right Middle
	m_fields[c][b].up = &m_fields[c][a];
	m_fields[c][b].down = &m_fields[c][c];
	if (c > 4) m_fields[c][b].left = &m_fields[c - 1][b];
	if (c < FIELD_SIZE - 1) m_fields[c][b].right = &m_fields[c + 1][b];

	// Lower Middle
	if (c > 4) m_fields[b][c].up = &m_fields[b][c - 1];
	if (c < FIELD_SIZE - 1) m_fields[b][c].down = &m_fields[b][c + 1];
	m_fields[b][c].left = &m_fields[a][c];
	m_fields[b][c].right = &m_fields[c][c];

}

State::State() {

	for (int i = 0; i < FIELD_SIZE; i++){
		std::vector<Field> v;
		m_fields.push_back(v);
		for (int j = 0; j < FIELD_SIZE; j++){
			m_fields[i].push_back(Field(i, j));
		}
	}

	fill(0, 3, 6);
	fill(1, 3, 5);
	fill(2, 3, 4);

	for (int i = 0; i < FIELD_SIZE; i++) m_fields[i][3].color = COLOR_NONE;
	
	// Exclude the field in the middle
	m_fields[3][3].color = -1;

	m_currenGameState = STATE_PLACE;
	m_currentPlayer = COLOR_WHITE;
	m_currentTurn = 0;
	m_blocked = false;
}

State::State(const State &_state){
	m_fields = _state.m_fields;
	m_currentPlayer = _state.m_currentPlayer;
	m_currenGameState = _state.m_currenGameState;
	m_currentTurn = _state.m_currentTurn;
	m_lastMill = _state.m_lastMill;
	m_blocked = _state.m_blocked;
}

void State::set(int _x, int _y, int _color) {
	m_fields[_x][_y].color = _color;
}

Field* State::get(int _x, int _y) {
	return &m_fields[_x][_y];
}


/*
	Returns the amout of stones of a player
*/
int State::stoneAmount(int _color) {
	int amount = 0;

	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){
			if (m_fields[i][j].color == _color){
				amount++;
			}
		}
	}

	return amount;
}

/*
	Returns the amount of movable stones of a player
*/
int State::stonesMovable(int _color){
	int amount = 0;

	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){
			if (m_fields[i][j].color == _color){

				if ((m_fields[i][j].left != NULL && m_fields[i][j].left->color == COLOR_NONE) ||
					(m_fields[i][j].right != NULL && m_fields[i][j].right->color == COLOR_NONE) ||
					(m_fields[i][j].up != NULL && m_fields[i][j].up->color == COLOR_NONE) ||
					(m_fields[i][j].down != NULL && m_fields[i][j].down->color == COLOR_NONE))
				{
					amount++;
				}
			}
		}
	}

	return amount;
}

/*
	Returns true if a position is part of a mill
*/
bool State::partOfMill(int _x, int _y){
	int color = get(_x, _y)->color;

	int startX = 0;
	int endX = FIELD_SIZE;
	int startY = 0;
	int endY = FIELD_SIZE;

	// Check if stone lies in one of the middle lanes..
	if (_y == 3){
		if (_x < 3){
			endX = 3;
		}
		else{
			startX = 4;
		}
	}
	if (_x == 3){
		if (_y < 3){
			endY = 3;
		}
		else{
			startY = 4;
		}
	}

	int counter = 0;

	for (int i = startX; i < endX; i++){
		if (m_fields[i][_y].color == color)
			counter++;
	}
	if (counter == 3)
		return true;

	counter = 0;

	for (int i = startY; i < endY; i++){
		if (m_fields[_x][i].color == color)
			counter++;
	}

	return counter == 3;
}

/*
	Returns if any stones of a player are removable
*/
bool State::stonesRemovable(int _color) {

	if (stoneAmount(_color) == 3)
		return true;

	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){
			if (m_fields[i][j].color == _color && partOfMill(i, j) == false){
				return true;
			}
		}
	}

	return false;
}

/*
* Returns the amount of mills on a state for a specified player
*/
int State::millAmount(int _color){
	int amount = 0;

	for (int i = 0; i < FIELD_SIZE; i++){
		// Middle Lane
		if (i == 3){
			if (m_fields[i][0].color == _color && m_fields[i][1].color == _color && m_fields[i][2].color == _color){
				amount++;
			}
			if (m_fields[i][4].color == _color && m_fields[i][5].color == _color && m_fields[i][6].color == _color){
				amount++;
			}
			if (m_fields[0][i].color == _color && m_fields[1][i].color == _color && m_fields[2][i].color == _color){
				amount++;
			}
			if (m_fields[4][i].color == _color && m_fields[5][i].color == _color && m_fields[6][i].color == _color){
				amount++;
			}
			continue;
		}

		// Horizontal
		int stones = 0;
		for (int j = 0; j < FIELD_SIZE; j++){
			if (m_fields[i][j].color == _color){
				stones++;
			}
		}
		if (stones == 3)
			amount++;

		// Vertical
		stones = 0;
		for (int j = 0; j < FIELD_SIZE; j++){
			if (m_fields[j][i].color == _color){
				stones++;
			}
		}

		if (stones == 3)
			amount++;
	}

	return amount;
}


void State::setPlayer(int _player) {
	m_currentPlayer = _player;
}

void State::setGameState(int _gameState) {
	m_currenGameState = _gameState;
}

void State::setTurn(int _turn) {
	m_currentTurn = _turn;
}

int State::getPlayer() {
	return m_currentPlayer;
}

int State::getGameState() {
	return m_currenGameState;
}

int State::getTurn() {
	return m_currentTurn;
}

std::vector<std::vector<Field>>* State::getFields() {
	return &m_fields;
}

void State::millSet(){
	m_lastMill = m_currentTurn;
}

int State::lastMill(){
	return m_lastMill;
}

void State::setBlocked(bool _blocked){
	m_blocked = _blocked;
}

bool State::isBlocked(){
	return m_blocked;
}
