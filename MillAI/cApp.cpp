#include "cApp.h"

cApp::cApp()
{
}

cApp::~cApp()
{
}

void cApp::Run()
{
	if (!Init()) 
		m_StateID = STATE_EXIT;
	else 
		m_StateID = STATE_GAME;
	
	m_CurrentState = new AS_Game(m_Graphics, m_Animations, m_Input, m_Sound);

	while (m_StateID != STATE_EXIT)
	{
		m_Input->handleInput();
		if (m_Input->isKeyReleased(SDLK_ESCAPE) || m_Input->appClosed())
		{
			m_StateID = STATE_EXIT;
		}

		m_CurrentState->Event();

		m_NextState = m_CurrentState->Update( /*m_Timer->GetDelta()*/ 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
		m_CurrentState->Draw();
		m_Graphics->swapWindow();

		ChangeState();

		SDL_Delay(16);
	}

	AL::Log::write(AL::LOG_INFORMATION, "App", "Shutting down..");
	
	delete m_Sound;
	SDL_Quit();
}

bool cApp::Init()
{
	AL::Log::reset();
	m_Graphics = new AL::Graphics();
	m_Animations = new AL::AnimationManager(m_Graphics);
	m_Input = new AL::Input();
	m_Sound = new AL::Sound();

	if (LoadConfig() == false)
	{
		AL::Log::write(AL::LOG_ERROR, "App Initialisation", "Could not load config file!");
		return false;
	}

	m_Graphics->init(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME.c_str(), NULL);
	SDL_ShowCursor(false);

	if (LoadResources() == false)
	{
		AL::Log::write(AL::LOG_ERROR, "App Initialisation", "Could not load resources!");
		return false;
	}

	m_Graphics->setFont("roboto", 32, AL::color(1, 1, 1));

	return true;
}

bool cApp::LoadConfig()
{
	Document config = JsonParser::LoadJSON("data/config.json");
	if (config == NULL || config.IsObject() == false)
	{
		AL::Log::write(AL::LOG_ERROR, "Resource Loader", "Could not load data/config.json !");
		return false;
	}

	if ((config.HasMember("AppName")
		&& config.HasMember("ScreenWidth")
		&& config.HasMember("ScreenHeight")
		&& config.HasMember("ResourceFile")) == false)
	{
		AL::Log::write(AL::LOG_ERROR, "Config Loader", "Could not load a needed value!");
		return false;
	}

	APP_NAME = config["AppName"].GetString();
	SCREEN_WIDTH = config["ScreenWidth"].GetInt();
	SCREEN_HEIGHT = config["ScreenHeight"].GetInt();
	RESOURCE_FILE = config["ResourceFile"].GetString();

	return true;
}

bool cApp::LoadResources()
{
	Document resources = JsonParser::LoadJSON(RESOURCE_FILE);
	if (resources == NULL)
	{
		AL::Log::write(AL::LOG_ERROR, "Resource Loader", "Could not load data/config.json !");
		return false;
	}

	for (int i = 0; i < resources["textures"].Size(); i++)
	{
		Value& obj = resources["textures"][i];
		if (m_Graphics->loadTexture(obj["src"].GetString(), obj["name"].GetString()) == false)
		{
			string error = "Could not load Texture '";
			error += obj["name"].GetString();
			error += "' from Path '";
			error += obj["src"].GetString();
			error += "'";
			AL::Log::write(AL::LOG_ERROR, "Resource Loader", error);
			return false;
		}
	}

	for (int i = 0; i < resources["fonts"].Size(); i++)
	{
		Value& obj = resources["fonts"][i];
		if (m_Graphics->loadFont(obj["src"].GetString(), obj["name"].GetString()) == false)
		{
			string error = "Could not load Font '";
			error += obj["name"].GetString();
			error += "' from Path '";
			error += obj["src"].GetString();
			error += "'";
			AL::Log::write(AL::LOG_ERROR, "Resource Loader", error);
			return false;
		}
	}

	if (resources["sounds"] != NULL){
		for (int i = 0; i < resources["sounds"].Size(); i++)
		{
			Value& obj = resources["sounds"][i];
			if (m_Sound->loadSound(obj["src"].GetString(), obj["name"].GetString()) == false)
			{
				string error = "Could not load sound '";
				error += obj["name"].GetString();
				error += "' from Path '";
				error += obj["src"].GetString();
				error += "'";
				AL::Log::write(AL::LOG_ERROR, "Resource Loader", error);
				return false;
			}
		}
	}	

	return true;
}

void cApp::ChangeState()
{
	if (m_NextState == STATE_NULL) 
		return;

	if (m_NextState != STATE_EXIT) 
		delete m_CurrentState;

	switch (m_NextState)
	{
	case STATE_GAME:
		m_CurrentState = new AS_Game(m_Graphics, m_Animations, m_Input, m_Sound);
		break;
	}

	m_StateID = m_NextState;
	m_NextState = STATE_NULL;
}