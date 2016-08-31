#include "FieldObject.h"


FieldObject::FieldObject(Field *_field, Rect _rect){
	m_Field = _field;
	m_Rect = _rect;
	m_hover = m_toMove = false;
}

FieldObject::~FieldObject(){

}

Field* FieldObject::getField(){
	return m_Field;
}

Rect FieldObject::getRect(){
	return m_Rect;
}

void FieldObject::setHover(bool _hover){
	m_hover = _hover;
}

bool FieldObject::getHover(){
	return m_hover;
}

void FieldObject::setToMove(bool _move){
	m_toMove = _move;
}

bool FieldObject::getToMove(){
	return m_toMove;
}