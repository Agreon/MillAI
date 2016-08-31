#pragma once

#include <string>

class GameObject
{
public:
	GameObject(std::string name, std::string type);
	~GameObject();

	int getObjectID(){ return m_ObjectID; }
protected:
	int m_ObjectID;
	std::string m_Name;
	std::string m_Type;
};

