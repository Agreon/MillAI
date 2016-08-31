#include "Board.h"

Board::Board(State *_state, AL::Graphics *_graphics){
	m_Graphics = _graphics;
	m_boardState = STANDART;

	setState(_state);

}


Board::~Board(){
}

void Board::update(){

}

void Board::draw(){

	int width = 600, height = 600, offset = 50;

	//Background
	m_Graphics->setColor(AL::color(0.93, 0.77, 0.56));	
	m_Graphics->drawRectangle(0, 0, 630, 630);

	//Lines 
	m_Graphics->setColor(AL::color(0.54, 0.35, 0.17));

	// Horizontal Lines

	for (int i = 0; i < 3; i++){
		m_Graphics->drawRectangle(offset + 80 * i + 2, offset + 80 * i + 7, (6-(i*2)) * 80, 10);
		m_Graphics->drawRectangle(offset + 80 * i + 2, offset + 80 * (FIELD_SIZE - 1 - i ) + 7, (6-(i*2)) * 80, 10);
	}

	m_Graphics->drawRectangle(offset + 2, offset + 80 * 3 + 7, 2 * 80, 10);
	m_Graphics->drawRectangle(offset + 80*4 + 2, offset + 80 * 3 + 7, 2 * 80, 10);

	// Vertical Lines

	for (int i = 0; i < 3; i++){
		m_Graphics->drawRectangle(offset + 80 * i + 7, offset + 80 * i + 2, 10, (6 - (i * 2)) * 80);
		m_Graphics->drawRectangle(offset + 80 * (FIELD_SIZE - 1 - i) + 7, offset + 80 * i + 2, 10, (6 - (i * 2)) * 80);
	}

	m_Graphics->drawRectangle(offset + 80 * 3 + 7, offset + 2, 10, 2 * 80);
	m_Graphics->drawRectangle(offset + 80 * 3 + 7, offset + 80 * 4 + 2, 10, 2 * 80);


	int currentPlayer = m_currentState->getPlayer();
	int nextPlayer = currentPlayer == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE;

	if (blocked)
		return;

	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){
			
			m_Graphics->setColor(AL::color(1, 1, 1, 1));
			
			// Empty field
			if (m_fields[i][j]->getField()->color == COLOR_NONE){

				m_Graphics->setColor(AL::color(1, 1, 1, 1));
				m_Graphics->drawTexture("brown_place", m_fields[i][j]->getRect().x, m_fields[i][j]->getRect().y);

				// If hovered
				if (m_fields[i][j]->getHover()){
					m_Graphics->setColor(AL::color(1, 1, 1, 0.7));

					// If in placing phase, show a stonoe overlay at mouse position
					if (m_currentState->getGameState() == STATE_PLACE || m_boardState == MOVING){
						if (currentPlayer == COLOR_WHITE)
							m_Graphics->drawTexture("white_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
						else 
							m_Graphics->drawTexture("black_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
					}
				}
			}
			else if (m_fields[i][j]->getField()->color == COLOR_WHITE){

				// If the stone is movable and selected draw green stone
				if (m_fields[i][j]->getToMove()){
					m_Graphics->drawTexture("green_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
				}
				// If the stone is movable and hovered draw light green stone
				else if (m_fields[i][j]->getHover() && m_currentState->getGameState() == STATE_MOVE){
					m_Graphics->drawTexture("light_green_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
				}
				else if (m_fields[i][j]->getHover() && m_boardState == REMOVING && nextPlayer == COLOR_WHITE){
					m_Graphics->drawTexture("red_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
				}
				// Draw normal
				else{
					m_Graphics->drawTexture("white_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
				}
			}
			else if (m_fields[i][j]->getField()->color == COLOR_BLACK){
				// if its from the other player and the player is able to remove a stone, mark it red on hovering
				if (nextPlayer == COLOR_BLACK){
					if (m_fields[i][j]->getHover() && m_boardState == REMOVING){
						m_Graphics->drawTexture("red_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
						continue;
					}
				}
				m_Graphics->drawTexture("black_stone", m_fields[i][j]->getRect().x - 12, m_fields[i][j]->getRect().y - 12);
			}
		}
	}

	m_Graphics->setFontSize(26);
	m_Graphics->drawText(m_status1.c_str(), 80, 600);
	m_Graphics->drawText(m_status2.c_str(), 350, 600);
}

State* Board::getState(){
	return m_currentState;
}

void Board::setState(State *_state){
	m_currentState = _state;

	m_fields.clear();

	// Connect state fields to UI-fields
	for (int i = 0; i < FIELD_SIZE; i++){
		vector<FieldObject*> v;
		m_fields.push_back(v);
		for (int j = 0; j < FIELD_SIZE; j++){
			m_fields[i].push_back(new FieldObject(_state->get(i, j), Rect(i * 80 + 50, j * 80 + 50, 25, 25)));
		}
	}
}

vector<vector<FieldObject*>>* Board::getFields(){
	return &m_fields;
}

void Board::setBoardState(int _state){
	m_boardState = _state;
}

int Board::getBoardState(){
	return m_boardState;
}

void Board::setStatus1(string _text){
	m_status1 = _text;
}

void Board::setStatus2(string _text){
	m_status2 = _text;
}