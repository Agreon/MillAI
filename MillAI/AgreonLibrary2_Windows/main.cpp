/* 
 * File:   main.cpp
 * Author: daniel
 *
 * Created on February 2, 2015, 3:13 PM
 */

#include <iostream>
#include <memory>
#include "AL.h"

#include "GameObject.h"
#include <vector>

using namespace rapidjson;

void LoadJSON()
{
	Document jsondoc;
		
	FILE *pFile = fopen("data/gameobjects.json", "r");
	rapidjson::FileStream is(pFile);

	jsondoc.ParseStream<0>(is);
	
	if (jsondoc.HasParseError())
	{
		AL::Log::write(AL::LOG_ERROR,"JSON Parser","parse error!!");
		return;
	}

	std::vector<GameObject*> objects;

	for (int i = 0; i < jsondoc["GameObjects"].Size(); i++)
	{
		Value& object = jsondoc["GameObjects"][i];

		if (object["type"] == "enemy")
		{

		}
		else if (object["type"] == "player")
		{

		}
		else if (object["type"] == "item")
		{
	
		}

		AL::Log::write(AL::LOG_INFORMATION, "JSON Parser", "Loading object: " + std::string(object["name"].GetString()));

		objects.push_back(new GameObject(object["name"].GetString(), object["type"].GetString()));

		for (Value::ConstMemberIterator itr = object["attributes"].MemberBegin(); itr != object["attributes"].MemberEnd(); ++itr)
		{
			char buf[33];
			_itoa_s(itr->value.GetInt(), buf, 10);
			AL::Log::write(AL::LOG_INFORMATION, "JSON Parser", itr->name.GetString() + std::string(": ") + std::string(buf));
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		char buf[33];
		_itoa_s(objects[i]->getObjectID(), buf, 10);
		AL::Log::write(AL::LOG_INFORMATION, "JSON Parser", buf);
	}

}

int main(int argc, char** argv)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Event event;
	bool running = true;

	AL::Graphics *graphics = new AL::Graphics();
	AL::AnimationManager *animManager = new AL::AnimationManager(graphics);
	AL::Input *input = new AL::Input();
	AL::Sound *sound = new AL::Sound();

	graphics->init(800, 600, "Agreon Library 2", NULL);

	if (!graphics->loadTexture("data/agreon_logo.png", "agreon_logo")
		|| !graphics->loadTexture("data/player_right_anim.png", "player_anim")
		|| !graphics->loadFont("data/roboto.ttf", "roboto"))
	{
		running = false;
	}
	graphics->setFont("roboto", 50, AL::color(1, 1, 1, 0.2));

	animManager->createAnimation("player_anim", "player_anim", -1, 10, 0, 200, 200, 64, 64);

	if (!sound->loadSound("data/wolf_howl.wav", "wolf_howl"))
	{
		running = false;
	}

	sound->playSound("wolf_howl");
	sound->setVolume(10);


	LoadJSON();

    while(running)
    {
        input->handleInput();
        
        if(input->appClosed()) running = false;
        
        if(input->isKeyReleased(SDLK_ESCAPE))
        {
            running = false;
        }
        
        if(input->isKeyReleased('a')) running = false;
                 
        animManager->updateAll();
        
        glClear(GL_COLOR_BUFFER_BIT);
        graphics->setColor(AL::color(1,1,1,1));

        graphics->drawTexture("agreon_logo",0,0);      
        graphics->drawText("Agreon Library 2.0",200,500);
        graphics->setColor(AL::color(0.5,1,1,1));
        animManager->showAll();
        
        graphics->swapWindow();
    }
    
	AL::Log::write(AL::LOG_INFORMATION,"Engine","Shutting down..");

    SDL_Quit();
    return 0;
}
