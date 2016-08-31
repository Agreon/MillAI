/* 
 * File:   Animation.cpp
 * Author: daniel
 * 
 * Created on February 17, 2015, 12:22 AM
 */

#include "Animation.h"

namespace AL
{
    Animation::Animation()
    {
    }

    Animation::Animation(int repeat, int fps, int rotation, int x, int y, int fw, int fh, bool active)
    {
	m_Fps = fps;
	m_Rotation = rotation;
	m_X = x;
	m_Y = y;
	m_FrameWidth = fw;
	m_FrameHeight = fh;
	m_Active = active;
	m_Texture = "";

	if(repeat != -1) m_Repeatations = repeat - 1;
	else m_Repeatations = -1;

	m_Currentframe = 0;
	m_Lastupdate = SDL_GetTicks();
	m_IsDone = false;
	m_IsPaused = false;
    }

    void Animation::setTexture( const std::string texture, uint ID )
    {
	m_Texture = texture;

	/// Get the number of frames
	int tmpW;
	glBindTexture(GL_TEXTURE_2D,ID);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tmpW);
	m_Frames = tmpW / m_FrameWidth;
    }

    void Animation::update()
    {
	if(m_IsPaused || m_Active == false) return;

	if( 1000 / m_Fps < (SDL_GetTicks() - m_Lastupdate))
	{
            m_Currentframe++;
            m_Lastupdate = SDL_GetTicks();

            if(m_Currentframe >= m_Frames)
            {
                if( m_Repeatations != -1)
		{
                    if(m_Repeatations == 0)
                    {
			m_IsDone = true;
                    }
                    else
                    {
                        m_Repeatations--;
                    }
                }
                m_Currentframe = 0;
            }
	}
    }


    void Animation::show(Graphics* Graphics)
    {
	if(m_Active == false) return;
	Graphics->drawSection( m_Texture, m_Currentframe * m_FrameWidth, 0, m_FrameWidth, m_FrameHeight, m_X, m_Y, m_Rotation );
    }


    void Animation::stop()
    {
	m_IsDone = true;
    }


    void Animation::pause()
    {
        m_IsPaused = true;
    }


    void Animation::resume()
    {
        m_IsPaused = false;
    }


    void Animation::activate()
    {
        m_Active = true;
    }


    void Animation::deactivate()
    {
        m_Active = false;
    }


    bool Animation::isDone()
    {
        return m_IsDone;
    }


    bool Animation::isPaused()
    {
        return m_IsPaused;
    }


    bool Animation::isActive()
    {
        return m_Active;
    }


    void Animation::setX(int x)
    {
        m_X = x;
    }


    void Animation::setY(int y)
    {
        m_Y = y;
    }


    void Animation::setXY(int x, int y)
    {
        m_X = x;
        m_Y = y;
    }

    void Animation::setRotation(int rotation)
    {
        m_Rotation = rotation;
    }

    void Animation::setActive(bool active)
    {
        m_Active = active;
    }

    int Animation::getRotation()
    {
        return m_Rotation;
    }

    std::string Animation::getTexture()
    {
        return m_Texture;
    }

//----------------------------------------------//
//              AnimationManager        	//
//----------------------------------------------//

    AnimationManager::AnimationManager(Graphics* Graphics)
    {
        m_Graphics = Graphics;
    }


    void AnimationManager::createAnimation(const std::string name, const std::string texture, int repeats, int speed, int rotation, int x, int y, int fw, int fh)
    {
        uint textureID = m_Graphics->getUint(texture);

        m_Animations[name] = new Animation( repeats,speed,rotation,x,y,fw,fh,true);
        m_Animations[name]->setTexture( texture, textureID );
    }

    void AnimationManager::updateAll()
    {
		for(std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end(); it++)
		{
			m_Animations[it->first]->update();

			if(m_Animations[it->first]->isDone())
			{
				delete m_Animations[it->first];
				m_Animations.erase(it->first);
			}
		}
    }


    void AnimationManager::showAll()
    {
		for(std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end(); it++)
		{
			m_Animations[it->first]->show( m_Graphics );
		}
    }


    void AnimationManager::stopAll()
    {
		for(std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end(); it++)
		{
			m_Animations[it->first]->stop();
		}
    }


    void AnimationManager::pauseAll()
    {
		for(std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end(); it++)
		{
			m_Animations[it->first]->pause();
		}
    }


    void AnimationManager::resumeAll()
    {
		for(std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end(); it++)
		{
			m_Animations[it->first]->resume();
		}
    }


    void AnimationManager::shutdown()
    {
		for (std::map<const std::string, Animation*>::iterator it=m_Animations.begin(); it!=m_Animations.end();)
		{
			m_Animations.erase(it++);
		}
    }


    void AnimationManager::update(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
			m_Animations[ID]->update();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to update with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::show(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->show( m_Graphics );
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to show with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::stop(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->stop();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to stop with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::pause(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->pause();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to pause with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::resume(const std::string ID)
    {
        std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->resume();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to resume with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::destroy(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				delete m_Animations[ID];
				m_Animations.erase(ID);
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to destroy with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::activate(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->activate();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to activate with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::deactivate(const std::string ID)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->deactivate();
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to deactivate with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::setX(const std::string ID, int x)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->setX(x);
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to setX with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::setY(const std::string ID, int y)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
				m_Animations[ID]->setY(y);
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to setY with this ID! ID: ") + ID);
		}
    }


    void AnimationManager::setPosition(const std::string ID, int x, int y)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
			m_Animations[ID]->setXY(x,y);
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to setXY with this ID! ID: ") + ID);
		}
    }

    void AnimationManager::setRotation(const std::string ID, int rotation)
    {
		std::map<std::string, Animation*>::iterator iter = m_Animations.find(ID);
		if (iter != m_Animations.end() )
		{
			m_Animations[ID]->setRotation(rotation);
		}
		else
		{
			Log::write(AL::LOG_ERROR, "AnimationManager", std::string("There is no Animation to set rotation with this ID! ID: ") + ID);
		}
    }

    int AnimationManager::getRotation(const std::string ID)
    {
        return m_Animations[ID]->getRotation();
    }  
}


