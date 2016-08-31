#include "GameObject.h"

static int GO_ID;

GameObject::GameObject(std::string name, std::string type)
{
	m_ObjectID = GO_ID++;
	m_Name = name;
	m_Type = type;
}


GameObject::~GameObject()
{
}
