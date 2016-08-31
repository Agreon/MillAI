#pragma once

#include "AgreonLibrary2_Windows\AL.h"
#include "State.h"
#include "FieldObject.h"

using namespace std;

enum BOARD_STATE{
	STANDART,
	MOVING,
	REMOVING
};

class Board
{
public:
	Board(State *_state, AL::Graphics *m_Graphics);
	~Board();

	void update();
	void draw();

	State *getState();
	void setState(State *_state);

	vector<vector<FieldObject*>>* getFields();

	void setBoardState(int _state);
	int getBoardState();

	void setStatus1(string _text);
	void setStatus2(string _text);

	bool blocked = false;

private:
	AL::Graphics *m_Graphics;
	State *m_currentState;
	vector<vector<FieldObject*>> m_fields;	

	int m_boardState;
	string m_status1, m_status2;

};

