#pragma once

#include "Player.h"

class Human : public Player
{
public:
	Human(AL::Input *_input, AL::Sound *_sound);
	~Human();

	void turn();
private:
	bool hovers(int _x, int _y, Rect _target);
	void validateSelection(FieldObject *_current);

	AL::Input *m_input;
	AL::Sound *m_sound;

	bool m_toRemove;
	FieldObject* m_fromField;
};

