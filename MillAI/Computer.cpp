#include "Computer.h"

high_resolution_clock::time_point start;

Computer::Computer()
{
}


Computer::~Computer()
{
}

void Computer::turn(){
	
	if (m_board->getState()->isBlocked() || m_color != m_board->getState()->getPlayer())
		return;

	int nextPlayer = (m_color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;

	
	calculate(m_board->getState());

	m_board->getState()->setPlayer(nextPlayer);
}

/*
	TODO: Comment
*/
void Computer::calculate(State *_state){
	_state->setPlayer(m_color);

	int currentPlayer = m_color;
	int nextPlayer = (m_color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
	
	int max = -10001, depth = 6, alpha = -10000, beta = 10000, ret, tmpX = -1, tmpY = -1;

	std::vector<Move> moves;	

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	start = high_resolution_clock::now();


	// Collect best moves depending on GameState
	// Moving
	if (_state->getGameState() == STATE_PLACE){
		moves = placing(_state, nextPlayer);
	}
	// Jumping
	else if (_state->stoneAmount(m_color) == 3){
		moves = jumping(_state, nextPlayer);
	}
	// Moving
	else if (_state->getGameState() == STATE_MOVE){
		moves = moving(_state, nextPlayer);
	}
	
	Move finalTurn;
	
	// Check for equal moves
	if (moves.size() > 1){

		std::cout << "Checking for equal moves (" << moves.size() << ")" << std::endl;

		// Check the best moves independently with depth = 1
		std::vector<Move> maxmoves = getMaxMoves(_state, moves);

		// If there are still some equals, select a random
		if (maxmoves.size() > 1){
			std::cout << "Multiple possibilities.. selecting random from " << maxmoves.size() << " moves" << std::endl;
			int random = (int)(rand() % maxmoves.size());
			finalTurn = maxmoves[random];
		}
		else{
			finalTurn = maxmoves[0];
		}
	}
	else{
		finalTurn = moves[0];
	}

	std::cout << "Final" << std::endl;

	if (finalTurn.fromX != -1){
		std::cout << "Moving: " << finalTurn.fromX << "," << finalTurn.fromY << std::endl;
		_state->get(finalTurn.fromX, finalTurn.fromY)->color = COLOR_NONE;
	}

	std::cout << "Setting: " << finalTurn.toX << "," << finalTurn.toY << std::endl;
	_state->get(finalTurn.toX, finalTurn.toY)->color = m_color;

	if (finalTurn.killX != -1){
		_state->set(finalTurn.killX, finalTurn.killY, COLOR_NONE);
		_state->millSet();
		std::cout << "Removed " << finalTurn.killX << "," << finalTurn.killY << std::endl;
	}


	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	std::cout << "Computer calculated turn in " << (double)duration_cast<milliseconds>(t2 - t1).count() / 1000 << std::endl;
}

std::vector<Move> Computer::placing(State *_state, int _nextPlayer){
	std::vector<Move> moves;

	int max = -10001, depth = 6, alpha = -10000, beta = 10000;

	depth = 4; //

	Move retMove;

	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){
			if (_state->get(i, j)->color == COLOR_NONE){
				_state->set(i, j, m_color);

				retMove = Move();

				// If a stone is to be removed, get the best option
				if (_state->partOfMill(i, j) && _state->stonesRemovable(_nextPlayer)){
					retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
				}
				else{
					retMove.value = -negamax(*_state, depth, -beta, -alpha);
				}

				_state->set(i, j, COLOR_NONE);

				std::cout << i << "," << j << ": " << retMove.value << std::endl;

				if (retMove.value > max){
					max = retMove.value;
					moves.clear();
				}
				// Add to list if equal, so that the computer can choose one
				if (retMove.value >= max){
					retMove.toX = i;
					retMove.toY = j;
					moves.push_back(retMove);
				}
			}
		}
	}

	return moves;
}

std::vector<Move> Computer::moving(State *_state, int _nextPlayer){
	std::vector<Move> moves;

	int max = -10001, depth = 8, alpha = -10000, beta = 10000;

	depth = 6; //

	Move retMove;

	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			if (_state->get(i, j)->color == m_color) {
				Field *current = _state->get(i, j);
				current->color = COLOR_NONE;

				// Up
				if (current->up != NULL && current->up->color == COLOR_NONE){
					current->up->color = m_color;

					retMove = Move();

					if (_state->partOfMill(i, current->up->y) && _state->stonesRemovable(_nextPlayer)){
						retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
					}
					else{
						retMove.value = -negamax(*_state, depth, -beta, -alpha);
					}

					current->up->color = COLOR_NONE;

					if (retMove.value > max){
						max = retMove.value;
						moves.clear();
					}
					if (retMove.value >= max){
						retMove = Move(i, j, i, current->up->y, retMove.killX, retMove.killY, max);
						moves.push_back(retMove);
					}
					std::cout << i << "," << j << "=>" << i << "," << current->up->y << ": " << retMove.value << std::endl;
				}

				//Down
				if (current->down != NULL && current->down->color == COLOR_NONE){
					current->down->color = m_color;

					retMove = Move();

					if (_state->partOfMill(i, current->down->y) && _state->stonesRemovable(_nextPlayer)){
						retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
					}
					else{
						retMove.value = -negamax(*_state, depth, -beta, -alpha);
					}

					current->down->color = COLOR_NONE;

					if (retMove.value > max){
						max = retMove.value;
						moves.clear();
					}
					if (retMove.value >= max){
						retMove = Move(i, j, i, current->down->y, retMove.killX, retMove.killY, max);
						moves.push_back(retMove);
					}

					std::cout << i << "," << j << "=>" << i << "," << current->down->y << ": " << retMove.value << std::endl;
				}

				//Left
				if (current->left != NULL && current->left->color == COLOR_NONE){
					current->left->color = m_color;

					retMove = Move();

					if (_state->partOfMill(current->left->x, j) && _state->stonesRemovable(_nextPlayer)){
						retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
					}
					else{
						retMove.value = -negamax(*_state, depth, -beta, -alpha);
					}

					current->left->color = COLOR_NONE;

					if (retMove.value > max){
						max = retMove.value;
						moves.clear();
					}
					if (retMove.value >= max){
						retMove = Move(i, j, current->left->x, j, retMove.killX, retMove.killY, max);
						moves.push_back(retMove);
					}

					std::cout << i << "," << j << "=>" << current->left->x << "," << j << ": " << retMove.value << std::endl;
				}

				// Right
				if (current->right != NULL && current->right->color == COLOR_NONE){
					current->right->color = m_color;

					retMove = Move();

					if (_state->partOfMill(current->right->x, j) && _state->stonesRemovable(_nextPlayer)){
						retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
					}
					else{
						retMove.value = -negamax(*_state, depth, -beta, -alpha);
					}

					current->right->color = COLOR_NONE;

					if (retMove.value > max){
						max = retMove.value;
						moves.clear();
					}
					if (retMove.value >= max){
						retMove = Move(i, j, current->right->x, j, retMove.killX, retMove.killY, max);
						moves.push_back(retMove);
					}

					std::cout << i << "," << j << "=>" << current->right->x << "," << j << ": " << retMove.value << std::endl;
				}
				current->color = m_color;
			}
		}
	}
	return moves;
}

std::vector<Move> Computer::jumping(State *_state, int _nextPlayer){
	std::vector<Move> moves;

	int max = -10001, depth = 6, alpha = -10000, beta = 10000;

	Move retMove;

	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			if (_state->get(i, j)->color == m_color) {
				Field *current = _state->get(i, j);
				current->color = COLOR_NONE;

				for (int k = 0; k < FIELD_SIZE; k++) {
					for (int l = 0; l < FIELD_SIZE; l++) {
						if (_state->get(k, l)->color == COLOR_NONE && k != i && l != j){

							Field *to = _state->get(k, l);
							to->color = m_color;

							retMove = Move();

							if (_state->partOfMill(to->x, to->y) && _state->stonesRemovable(_nextPlayer)){
								retMove = removeStone(_state, depth, alpha, beta, _nextPlayer);
							}
							else{
								retMove.value = -negamax(*_state, depth, -beta, -alpha);
							}

							_state->set(k, l, COLOR_NONE);

							if (retMove.value > max){
								max = retMove.value;
								moves.clear();
							}
							if (retMove.value >= max){
								retMove = Move(i, j, to->x, to->y, retMove.killX, retMove.killY, max);
								moves.push_back(retMove);
							}

							std::cout << i << "," << j << "=>" << k << "," << l << ": " << retMove.value << std::endl;
						}
					}
				}
				current->color = m_color;
			}
		}
	}

	return moves;
}

int Computer::negamax(State &_state, int _depth, int _alpha, int _beta){

	_depth -= 1;

	int val = -10000;

	int currentPlayer = _state.getPlayer();
	int nextPlayer = currentPlayer == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE;
	
	// GameState depending attribute checking
	if (_state.getGameState() == STATE_PLACE) {
		if (_state.getTurn() == GAME_CHANGE_TURNS && _state.getPlayer() != m_color){
			_state.setGameState(STATE_MOVE);
		}
	}
	else {
		// Checks if an winning or loosing state is reached. Depth is added, so that nearer goal states are seeked.
		if (_state.stoneAmount(currentPlayer) < 3 || _state.stonesMovable(currentPlayer) < 1) {
			return 10000 + _depth + 1;
		}

		if (_state.stoneAmount(nextPlayer) < 3 || _state.stonesMovable(nextPlayer) < 1) {
			return -10000 - _depth;
		}
	}

	// Returns State Value
	if (_depth == 0){
		val = getStateValue(_state, currentPlayer);
		val -= getStateValue(_state, nextPlayer);
		if (currentPlayer != m_color) val *= -1;
		return val;
	}

	_state.setPlayer(nextPlayer);

	int ret = 0, max = -10000;

	if (_state.getGameState() == STATE_PLACE) {
		for (int i = 0; i < FIELD_SIZE; i++) {
			for (int j = 0; j < FIELD_SIZE; j++) {
				if (_state.get(i, j)->color == COLOR_NONE) {
					_state.set(i, j, nextPlayer);

					// If a stone is to be removed
					if (_state.partOfMill(i, j) && _state.stonesRemovable(currentPlayer)){

						ret = removeStoneVal(_state,_depth,_alpha,_beta,currentPlayer);
					}
					else{
						ret = -negamax(_state, _depth, -_beta, -_alpha);
					}

					_state.set(i, j, COLOR_NONE);

					if (ret >= _beta) {
						_state.setPlayer(currentPlayer);
						return _beta;
					}
					if (ret > _alpha)
						_alpha = val;

					if (ret > val)
						val = ret;
				}
			}
		}
	}
	// Jumping
	/*else if (_state.stoneAmount(nextPlayer) == 3){
		for (int i = 0; i < FIELD_SIZE; i++) {
			for (int j = 0; j < FIELD_SIZE; j++) {
				if (_state.get(i, j)->color == nextPlayer) {
					Field *current = _state.get(i, j);
					current->color = COLOR_NONE;

					// Check every free place
					for (int k = 0; k < FIELD_SIZE; k++) {
						for (int l = 0; l < FIELD_SIZE; l++) {
							if (_state.get(k, l)->color == COLOR_NONE && k != i && l != j) {

								Field *to = _state.get(k, l);
								to->color = nextPlayer;

								if (_state.partOfMill(to->x, to->y) && _state.stonesRemovable(currentPlayer)) {
									ret = removeStoneVal(_state, _depth, _alpha, _beta, currentPlayer);
								}
								else
									ret = -negamax(_state, _depth, -_beta, -_alpha);

								if (ret > max) {
									max = ret;
								}
								_state.set(k, l, COLOR_NONE);
							}
						}
					}
					current->color = nextPlayer;

					if (max >= _beta){
						_state.setPlayer(currentPlayer);
						return _beta;
					}
					if (max > _alpha)
						_alpha = val;

					if (max > val)
						val = max;
				}
			}
		}
	}*/
	// Moving
	else if (_state.getGameState() == STATE_MOVE){
		for (int i = 0; i < FIELD_SIZE; i++) {
			for (int j = 0; j < FIELD_SIZE; j++) {
				if (_state.get(i, j)->color == nextPlayer) {
					Field *current = _state.get(i, j);
					current->color = COLOR_NONE;


					if (current->up != NULL && current->up->color == COLOR_NONE){
						current->up->color = nextPlayer;

						if (_state.partOfMill(i, current->up->y) && _state.stonesRemovable(currentPlayer)){
							ret = removeStoneVal(_state, _depth, _alpha, _beta, currentPlayer);
						}
						else
							ret = -negamax(_state, _depth, -_beta, -_alpha);

						current->up->color = COLOR_NONE;

						if (ret > max){
							max = ret;
						}
					}

					if (current->down != NULL && current->down->color == COLOR_NONE){
						current->down->color = nextPlayer;

						if (_state.partOfMill(i, current->down->y) && _state.stonesRemovable(currentPlayer))
							ret = removeStoneVal(_state, _depth, _alpha, _beta, currentPlayer);
						else
							ret = -negamax(_state, _depth, -_beta, -_alpha);

						current->down->color = COLOR_NONE;

						if (ret > max){
							max = ret;
						}
					}


					if (current->left != NULL && current->left->color == COLOR_NONE){
						current->left->color = nextPlayer;

						if (_state.partOfMill(current->left->x, j) && _state.stonesRemovable(currentPlayer))
							ret = removeStoneVal(_state, _depth, _alpha, _beta, currentPlayer);
						else
							ret = -negamax(_state, _depth, -_beta, -_alpha);

						current->left->color = COLOR_NONE;

						if (ret > max){
							max = ret;
						}
					}


					if (current->right != NULL && current->right->color == COLOR_NONE){
						current->right->color = nextPlayer;

						if (_state.partOfMill(current->right->x, j) && _state.stonesRemovable(currentPlayer))
							ret = removeStoneVal(_state, _depth, _alpha, _beta, currentPlayer);
						else
							ret = -negamax(_state, _depth, -_beta, -_alpha);

						current->right->color = COLOR_NONE;

						if (ret > max){
							max = ret;
						}
					}

					current->color = nextPlayer;

					if (max >= _beta){
						_state.setPlayer(currentPlayer);
						return _beta;
					}
					if (max > _alpha)
						_alpha = val;

					if (max > val)
						val = max;

				}
			}
		}
	}

	_state.setPlayer(currentPlayer);
	return _alpha;
}

/*
	Tests every move that removes a stone from the enemy with a negamax and returns the best value.
*/
int Computer::removeStoneVal(State &_state, int _depth, int _alpha, int _beta, int _player){
	int ret, val = -10000;

	int playerStoneAmount = _state.stoneAmount(_player);

	for (int k = 0; k < FIELD_SIZE; k++){
		for (int l = 0; l < FIELD_SIZE; l++){
			if (_state.get(k, l)->color == _player && (_state.partOfMill(k, l) == false || playerStoneAmount == 3)){

				_state.set(k, l, COLOR_NONE);
				ret = -negamax(_state, _depth, -_beta, -_alpha);
				_state.set(k, l, _player);

				if (ret > val){
					val = ret;
				}
			}
		}
	}

	return val;
}


Move Computer::removeStone(State *_state, int _depth, int _alpha, int _beta, int _player){
	Move move;

	move.value = -10000;

	int ret2 = 0;

	int playerStoneAmount = _state->stoneAmount(_player);

	for (int k = 0; k < FIELD_SIZE; k++){
		for (int l = 0; l < FIELD_SIZE; l++){
			if (_state->get(k, l)->color == _player && (_state->partOfMill(k, l) == false || playerStoneAmount == 3)){

				_state->set(k, l, COLOR_NONE);
				ret2 = -negamax(*_state, _depth, -_beta, -_alpha);
				_state->set(k, l, _player);

				if (ret2 > move.value){
					move.killX = k;
					move.killY = l;
					move.value = ret2;
				}
			}
		}
	}
	return move;
}


/*
	Returns the heuristik of a given state for a specific player
	Amount of Stones
	General and free directions of a field
*/
int Computer::getStateValue(State &_state, int _player){
	int val = 0;

	// Stone amount
	val += _state.stoneAmount(_player) * 30;

	int ways = 0;

	// Move-possibilities
	for (int i = 0; i < FIELD_SIZE; i++){
		for (int j = 0; j < FIELD_SIZE; j++){

			if (_state.get(i, j)->color == _player) {
				Field *current = _state.get(i, j);

				// Directions
				if (current->left != NULL) {
					val += 2;
					if (current->left->color == COLOR_NONE) {
						ways++;
					}
				}
				if (current->right != NULL) {
					val += 2;
					if (current->right->color == COLOR_NONE) {
						ways++;
					}
				}
				if (current->up != NULL) {
					val += 2;
					if (current->up->color == COLOR_NONE) {
						ways++;
					}
				}
				if (current->down != NULL) {
					val += 2;
					if (current->down->color == COLOR_NONE) {
						ways++;
					}
				}

			}

		}
	}
	val += ways * 3;

	return val;
}

/*
	evaluates the direct value of a set of moves and returns the best ones.
*/
std::vector<Move> Computer::getMaxMoves(State *_state, std::vector<Move> _moves){
	std::vector<Move> maxMoves;
	int max = -10000, ret;
	int nextPlayer = (m_color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;

	for (int i = 0; i < _moves.size(); i++){

		if (_moves[i].fromX != -1){
			_state->set(_moves[i].fromX, _moves[i].fromY, COLOR_NONE);
			std::cout << "Moving: " << _moves[i].fromX << "," << _moves[i].fromY << std::endl;
		}
		_state->set(_moves[i].toX, _moves[i].toY, m_color);
		std::cout << "Setting: " << _moves[i].toX << "," << _moves[i].toY << std::endl;

		if (_moves[i].killX != -1){
			_state->set(_moves[i].killX, _moves[i].killY, COLOR_NONE);
			std::cout << "Removed " << _moves[i].killX << "," << _moves[i].killY << std::endl;
		}


		ret = getStateValue(*_state, m_color);
		ret -= getStateValue(*_state, nextPlayer);

		std::cout << "RET: " << ret << std::endl;

		if (ret > max){
			max = ret;
			maxMoves.clear();
		}
		if (ret >= max){
			maxMoves.push_back(_moves[i]);
		}

		if (_moves[i].fromX != -1){
			_state->set(_moves[i].fromX, _moves[i].fromY, m_color);
		}
		_state->set(_moves[i].toX, _moves[i].toY, COLOR_NONE);
		if (_moves[i].killX != -1){
			_state->set(_moves[i].killX, _moves[i].killY, nextPlayer);
		}
	}

	return maxMoves;
}