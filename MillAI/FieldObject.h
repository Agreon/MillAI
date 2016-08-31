#pragma once

#include "AgreonLibrary2_Windows\AL.h"
#include "State.h"

class FieldObject
{
public:
	FieldObject(Field *_field, Rect _rect);
	~FieldObject();

	Field* getField();
	Rect getRect();

	void setHover(bool _hover);
	bool getHover();

	void setToMove(bool _move);
	bool getToMove();
private:
	Field* m_Field;
	Rect m_Rect;
	bool m_hover;
	bool m_toMove;
};

