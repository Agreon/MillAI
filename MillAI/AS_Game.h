#pragma once

#include <string>

#include "AgreonLibrary2_Windows\AL.h"
#include "definitions.h"
#include "AppState.h"
#include "Human.h"
#include "Computer.h"

using namespace std;

class AS_Game : public AppState
{
public:
	AS_Game(AL::Graphics *_graphics, AL::AnimationManager *_animManager, AL::Input *_input, AL::Sound *_sound);
	~AS_Game();

	void init();
	void resetBoard();

	void Event();
	int	Update(double _delta);
	void Draw();
private:
	Board *m_board;
	State *m_currentState;
	Player *m_currentPlayer;

	vector<Player*> m_players;

	int m_whiteWins, m_blackWins;
	bool m_gameFinished;
};

