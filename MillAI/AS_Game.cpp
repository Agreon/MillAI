#include "AS_Game.h"


AS_Game::AS_Game(AL::Graphics *_graphics, AL::AnimationManager *_animManager, AL::Input *_input, AL::Sound *_sound)
{
	m_Graphics = _graphics;
	m_Animations = _animManager;
	m_Input = _input;
	m_Sound = _sound;

	m_currentState = new State();
	m_board = new Board(m_currentState, m_Graphics);

	m_NextState = STATE_NULL;

	m_whiteWins = m_blackWins = 0;
	m_gameFinished = false;

	init();	
}


AS_Game::~AS_Game()
{
}

void AS_Game::init(){
	m_players.push_back(new Human(m_Input, m_Sound));
	m_players.push_back(new Computer());

	m_players[0]->setColor(COLOR_WHITE);
	m_players[1]->setColor(COLOR_BLACK);

	m_players[0]->setBoard(m_board);
	m_players[1]->setBoard(m_board);

	m_currentPlayer = m_players[0];
	m_currentState->setPlayer(m_currentPlayer->getColor());
}

void AS_Game::resetBoard(){
	m_currentState = new State(); 
	m_currentPlayer = m_players[0];	
	m_currentState->setPlayer(m_currentPlayer->getColor());
	m_board = new Board(m_currentState, m_Graphics);
	m_players[0]->setBoard(m_board);
	m_players[1]->setBoard(m_board);
}


void AS_Game::Event()
{
}


int AS_Game::Update(double _delta)
{
	// So that the player can see what happened
	if (m_gameFinished){
		Sleep(3000);
		resetBoard();
		m_gameFinished = false;
	}

	if (m_board->getState()->isBlocked())
		return STATE_NULL;

	m_currentPlayer->turn();

	// Check if the player of the state has been changed
	if (m_currentPlayer->getColor() != m_board->getState()->getPlayer()){
		if (m_currentPlayer == m_players[0]){
			m_currentPlayer = m_players[1];
		}
		else{
			m_currentPlayer = m_players[0];
			m_board->getState()->setTurn(m_board->getState()->getTurn() + 1);
		}

		// Change GameState 
		if (m_board->getState()->getTurn() == GAME_CHANGE_TURNS){
			m_board->getState()->setGameState(STATE_MOVE);
		}

		// Computers turn
		if (dynamic_cast<Computer*>(m_currentPlayer)){
			m_board->setStatus1("Computer calculating..");
			m_board->setStatus2("");

		}
		// Player information
		else{
			if (m_board->getState()->getGameState() == STATE_PLACE){
				m_board->setStatus1("Place a stone");
				m_board->setStatus2(to_string(GAME_CHANGE_TURNS - m_board->getState()->getTurn()) + " stones left");
			}
			else{
				m_board->setStatus1("Move a stone");
				m_board->setStatus2("");
			}
		}
	}


    // Check if somebody won
	if (m_board->getState()->getGameState() == STATE_MOVE){

        // Less than 3 stones
		if (m_board->getState()->stoneAmount(COLOR_WHITE) < 3){
			m_board->setStatus2("Black won!");
			m_blackWins++;
			m_gameFinished = true;
        } else
			if (m_board->getState()->stoneAmount(COLOR_BLACK) < 3){
			m_board->setStatus2("White won!");
			m_whiteWins++;
			m_gameFinished = true;
		} else
        // No movable stones left
		if (m_board->getState()->stonesMovable(COLOR_WHITE) < 1){
			m_board->setStatus2("Black won!");
			m_blackWins++;
			m_gameFinished = true;
		} else
			if (m_board->getState()->stonesMovable(COLOR_BLACK) < 1){
			m_board->setStatus2("White won!");
			m_whiteWins++;
			m_gameFinished = true;
		}

		// Remis if there was no Mill fpr 20 turns
		if (m_board->getState()->getTurn() - m_board->getState()->lastMill() > 20){
			m_board->setStatus2("Remis!");
			m_gameFinished = true;
		}
    }


	return m_NextState;
}

void AS_Game::Draw()
{
	m_board->draw();

	m_Graphics->setFontSize(26);
	m_Graphics->drawText(("White: " + string(to_string(m_whiteWins))).c_str(), 150, 30);

	m_Graphics->drawText(("Black: " + string(to_string(m_blackWins))).c_str(), 350, 30);

	m_Graphics->drawTexture("cursor", m_Input->getMousePositionX(), m_Input->getMousePositionY());
}
