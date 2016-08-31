/* 
 * File:   Input.cpp
 * Author: daniel
 * 
 * Created on February 5, 2015, 4:39 PM
 */

#include "Input.h"

namespace AL
{
    Input::Input()
    {
        m_MouseX = 0;
        m_MouseY = 0;
        
        for(int i = 0; i < 536; i++)
		{
			m_Keys[i] = NONE;
		}
        
        m_LeftMouse = NONE;
        m_RightMouse = NONE;
        
        bool m_MouseWheelUp = false;
        bool m_MouseWheelDown = false;  
        
        bool m_AppClose = false;        
    }

    Input::~Input()
    {
    }
    
    
    void Input::handleInput()
    {
        m_LeftMouse = NONE;
        m_RightMouse = NONE;
        m_MouseWheelUp = false;
        m_MouseWheelDown = false;
		m_AppClose = false;
        
        for(int i = 0; i < 536; i++)
		{
            m_Keys[i] = NONE;
		}
                
        while (SDL_PollEvent( &m_Evt ) )
        {
            switch( m_Evt.type )
            {
                case SDL_QUIT:
                    m_AppClose = true;
                    break;
                case SDL_KEYDOWN:
                    if(m_Evt.key.keysym.sym < 525) m_Keys[ m_Evt.key.keysym.sym ] = PRESSED;
                    else
                    {
                        switch(m_Evt.key.keysym.sym)
                        {
                            case SDLK_LEFT: m_Keys[525] = PRESSED; break;
                            case SDLK_RIGHT: m_Keys[526] = PRESSED; break;
                            case SDLK_UP: m_Keys[527] = PRESSED; break;
                            case SDLK_DOWN: m_Keys[528] = PRESSED; break;
                            case SDLK_LSHIFT: m_Keys[529] = PRESSED; break;
                            case SDLK_RSHIFT: m_Keys[530] = PRESSED; break;
                            case SDLK_LCTRL: m_Keys[531] = PRESSED; break;
                            case SDLK_RCTRL: m_Keys[532] = PRESSED; break;
                            case SDLK_LALT: m_Keys[533] = PRESSED; break;
                            case SDLK_RALT: m_Keys[534] = PRESSED; break;
                            default: break;
                        }
                    }
                    break;
                case SDL_KEYUP:                   
                    if(m_Evt.key.keysym.sym < 525) m_Keys[ m_Evt.key.keysym.sym ] = RELEASED;
                    else
                    {
                        switch(m_Evt.key.keysym.sym)
                        {
                            case SDLK_LEFT: m_Keys[525] = RELEASED; break;
                            case SDLK_RIGHT: m_Keys[526] = RELEASED; break;
                            case SDLK_UP: m_Keys[527] = RELEASED; break;
                            case SDLK_DOWN: m_Keys[528] = RELEASED; break;
                            case SDLK_LSHIFT: m_Keys[529] = RELEASED; break;
                            case SDLK_RSHIFT: m_Keys[530] = RELEASED; break;
                            case SDLK_LCTRL: m_Keys[531] = RELEASED; break;
                            case SDLK_RCTRL: m_Keys[532] = RELEASED; break;
                            case SDLK_LALT: m_Keys[533] = RELEASED; break;
                            case SDLK_RALT: m_Keys[534] = RELEASED; break;
                            default: break;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(m_Evt.button.button == SDL_BUTTON_LEFT)
                    {
                        m_LeftMouse = PRESSED;
                    }
                    else if(m_Evt.button.button == SDL_BUTTON_RIGHT)
                    {
                        m_RightMouse = PRESSED;
                    }
                    m_MouseX = m_Evt.button.x;
                    m_MouseY = m_Evt.button.y;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(m_Evt.button.button == SDL_BUTTON_LEFT)
                    {
                        m_LeftMouse = RELEASED;
                    }
                    else if(m_Evt.button.button == SDL_BUTTON_RIGHT)
                    {
                        m_RightMouse = RELEASED;
                    }
                    m_MouseX = m_Evt.button.x;
                    m_MouseY = m_Evt.button.y;
                    break;
                case SDL_MOUSEMOTION:
                     m_MouseX = m_Evt.motion.x;
                     m_MouseY = m_Evt.motion.y;
                     break;
            }
        }
    }
    
    bool Input::isKeyPressed( SDL_Keycode key )
    {
        if(key < 525) return m_Keys[ key ] == PRESSED;
        else
        {
            switch(m_Evt.key.keysym.sym)
            {
                case SDLK_LEFT: return m_Keys[525] == PRESSED;
                case SDLK_RIGHT:return m_Keys[526] == PRESSED;
                case SDLK_UP: return m_Keys[527] == PRESSED;
                case SDLK_DOWN: return m_Keys[528] == PRESSED;
                case SDLK_LSHIFT: return m_Keys[529] == PRESSED;
                case SDLK_RSHIFT: return m_Keys[530] == PRESSED;
                case SDLK_LCTRL: return m_Keys[531] == PRESSED;
                case SDLK_RCTRL: return m_Keys[532] == PRESSED;
                case SDLK_LALT: return m_Keys[533] == PRESSED;
                case SDLK_RALT: return m_Keys[534] == PRESSED;
                default: return false;
            }
        }
    }
    
    bool Input::isKeyReleased( SDL_Keycode key )
    {
        if(key < 525) return m_Keys[ key ] == RELEASED;
        else
        {
            switch(m_Evt.key.keysym.sym)
            {
                case SDLK_LEFT: return m_Keys[525] == RELEASED;
                case SDLK_RIGHT:return m_Keys[526] == RELEASED;
                case SDLK_UP: return m_Keys[527] == RELEASED;
                case SDLK_DOWN: return m_Keys[528] == RELEASED;
                case SDLK_LSHIFT: return m_Keys[529] == RELEASED;
                case SDLK_RSHIFT: return m_Keys[530] == RELEASED;
                case SDLK_LCTRL: return m_Keys[531] == RELEASED;
                case SDLK_RCTRL: return m_Keys[532] == RELEASED;
                case SDLK_LALT: return m_Keys[533] == RELEASED;
                case SDLK_RALT: return m_Keys[534] == RELEASED;
                default: return false;
            }
        }
    }

    bool Input::isKeyPressed( char key )
    {
        return m_Keys[key] == PRESSED;
    }

    bool Input::isKeyReleased(char key)
    {
        return m_Keys[key] == RELEASED;
    }
    
    bool Input::leftMousePressed()
    {
        return m_LeftMouse;
    }

	bool Input::leftMouseReleased()
	{
		return m_LeftMouse == RELEASED;
	}

    bool Input::rightMousePressed()
    {
        return m_RightMouse;
    }

	bool Input::rightMouseReleased()
	{
		return m_RightMouse == RELEASED;
	}

    bool Input::mouseWheelUp()
    {
        return m_MouseWheelUp;
    }

    bool Input::mouseWheelDown()
    {
        return m_MouseWheelDown;
    }

    int Input::getMousePositionX()
    {
        return m_MouseX;
    }

    int Input::getMousePositionY()
    {
        return m_MouseY;
    }
    
    bool Input::appClosed()
    {
        return m_AppClose; 
    }

}



