#include "Human.h"


Human::Human(AL::Input *_input, AL::Sound *_sound)
{
	m_input = _input;
	m_sound = _sound;
	m_toRemove = false;
	m_fromField = NULL;
}

Human::~Human()
{
}

/*
	Human turn gets called every frame
*/
void Human::turn(){

	if (m_board->getState()->getPlayer() != m_color)
		return;

	// Deselection
	if (m_input->rightMouseReleased()){
		if (m_fromField != NULL){
			m_fromField->setToMove(false);
			m_fromField = NULL;
		}
		return;
	}

	vector<vector<FieldObject*>> *fieldObjects = m_board->getFields();
	FieldObject *current = NULL;

	// Hovering
	for (AL::uint i = 0; i < fieldObjects->size(); i++){
		for (AL::uint j = 0; j < fieldObjects->size(); j++){

			current = (*fieldObjects)[i][j];
			current->setHover(false);

			Rect toCheck = current->getRect();

			// Player Stones are bigger than places
			if (current->getField()->color != COLOR_NONE){
				toCheck = Rect(toCheck.x - 12, toCheck.y - 12, toCheck.w + 25, toCheck.h + 25);
			}

			if (hovers(m_input->getMousePositionX(), m_input->getMousePositionY(), toCheck)){
				current->setHover(true);

				// On Click
				if (m_input->leftMouseReleased())
					validateSelection(current);

				return;
			}
		}
	}
}

/*
	Returns true, if a position intersects a Rect-Object
	=> Only used for Mouse-Field comparision
*/
bool Human::hovers(int _x, int _y, Rect _target){
	if (_y >= _target.y + _target.h) return false;
	if (_x >= _target.x + _target.w) return false;
	if (_y <= _target.y) return false;
	if (_x <= _target.x) return false;

	return true;
}

/*
	TODO:
	+ Sounds
*/
void Human::validateSelection(FieldObject *_current){

	Field *field = _current->getField();
	int nextPlayer = (m_color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
	
	// If a stone has to be removed
	if (m_board->getBoardState() == REMOVING){

		// False color
		if ((m_color == COLOR_WHITE && field->color != COLOR_BLACK) || (m_color == COLOR_BLACK && field->color != COLOR_WHITE)){
			m_board->setStatus2("Not the right color!");
			return;
		}

		// stone is part of mill
		if (m_board->getState()->partOfMill(field->x, field->y) && m_board->getState()->stoneAmount(nextPlayer) != 3){
			m_board->setStatus2("Stone is part of a mill!");
			return;
		}

		field->color = COLOR_NONE;
	
		m_board->setBoardState(STANDART);

		m_board->getState()->setPlayer(nextPlayer);

		//TODO: Play Sound removed
		m_sound->playSound("remove");
	}
	// Move to 
	else if (m_fromField != NULL){

		// Select new stone
		if (field->color == m_color){
			m_fromField->setToMove(false);
			m_fromField = _current;
			m_fromField->setToMove(true);
			return;
		}

		// If the player is not allowed to jump
		if (m_board->getState()->stoneAmount(m_color) != 3){
			// check if target is not a neighbour
			if (m_fromField->getField() != field->left &&
				m_fromField->getField() != field->right &&
				m_fromField->getField() != field->up &&
				m_fromField->getField() != field->down)
				return;
		}

		// If empty field, set stone
		if (field->color == COLOR_NONE){
			field->color = m_color;

			// Clear old
			m_fromField->getField()->color = COLOR_NONE;
			m_fromField->setToMove(false);
			m_fromField = NULL;

			// If the player created a mill
			if (m_board->getState()->partOfMill(field->x, field->y)){
				//m_toRemove = true;
				m_board->setBoardState(REMOVING);
				m_board->setStatus1("Remove a stone");
				m_board->getState()->millSet();
			}
			else{
				m_board->setBoardState(STANDART);
				m_board->getState()->setPlayer(nextPlayer);
			}

			//TODO: Play Sound
			m_sound->playSound("move");
		} 
	}
	// Place stone
	else if (m_board->getState()->getGameState() == STATE_PLACE && field->color == COLOR_NONE){
		field->color = m_color;

		// If the player created a mill
		if (m_board->getState()->partOfMill(field->x, field->y)){
			m_board->setBoardState(REMOVING);
			m_board->setStatus1("Remove a stone");
			m_board->setStatus2("");
		}
		else{
			m_board->getState()->setPlayer(nextPlayer);
		}

		//TODO: Play Sound
		m_sound->playSound("place");
	}
	// Select stone to move
	else if (m_board->getState()->getGameState() == STATE_MOVE && field->color == m_color){
		m_fromField = _current;
		m_fromField->setToMove(true);
		m_board->setBoardState(MOVING);
	}
}
