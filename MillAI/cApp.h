#pragma once

#include "AgreonLibrary2_Windows/AL.h"
#include "AppState.h"
#include "AS_Game.h"

using namespace rapidjson;
using namespace std;

class cApp
{
public:
	cApp();
	~cApp();

	void Run();
	bool Init();
	bool LoadConfig();
	bool LoadResources();

	void ChangeState();
private:
	AL::Graphics *m_Graphics;
	AL::AnimationManager *m_Animations;
	AL::Input *m_Input;
	AL::Sound *m_Sound;

	bool m_Running;

	AppState *m_CurrentState;
	int m_NextState;
	int m_StateID;

	string 	APP_NAME;
	int 	SCREEN_WIDTH;
	int 	SCREEN_HEIGHT;
	string	RESOURCE_FILE;
};