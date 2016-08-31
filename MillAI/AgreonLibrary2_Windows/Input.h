/* 
 * File:   Input.h
 * Author: daniel
 *
 * Created on February 5, 2015, 4:38 PM
 */

#ifndef INPUT_H
#define	INPUT_H

#include <iostream>
#include <SDL_events.h>

namespace AL
{     
	typedef unsigned short ushort;

    enum InputState
    {
        NONE,
        PRESSED,
        RELEASED
    };
        
    /**
     * Input
     * TODO: 
     * MouseWheelMovement
     * 
     */
    class Input
    {
    private:
        SDL_Event m_Evt;
        
        ushort m_MouseX;
        ushort m_MouseY;
        
        ushort m_Keys[536];
        
        ushort m_LeftMouse;
        ushort m_RightMouse;
        
        bool m_MouseWheelUp;
        bool m_MouseWheelDown;       
        
        bool m_AppClose;
    public:
        Input();
        ~Input();
        
        /**
		*   Pulls the Input from the eventlist.
		*/
        void handleInput();
        
        /**
		*   Checks if a key is pressed.
		*
		*   @param key	A key in SDL format.
		*   @returns    if key is pressed.
		*/
		bool isKeyPressed( SDL_Keycode key );
            
        /**
		*   Checks if a key is released.
		*   
		*   @param key	A key in SDL format.
		*   @returns    if key is released.
		*/
		bool isKeyReleased( SDL_Keycode key );
            
        /**
        *   Checks if a key is pressed.
        *
        *   @param key  A key as char.
        *   @return     if key is pressed.
        */
        bool isKeyPressed(char key);
        
        /**
        *   Checks if a key is released.
        * 
        *   @param key   A key as char.
        *   @return      if key is released.
        */
        bool isKeyReleased(char key);
        
		/**
		*   @returns if the left mouse button is pressed.
		*/
		bool leftMousePressed();

		/**
		*   @returns if the left mouse button is released.
		*/
		bool leftMouseReleased();
	
		/**
		*   @returns if the right mouse is pressed.
		*/
		bool rightMousePressed();

		/**
		*   @returns if the right mouse button is released.
		*/
		bool rightMouseReleased();

		/**
		*   @returns if the mouse wheel was moved upwards.
		*/	
		bool mouseWheelUp();

		/**
		*   @return if the mouse wheel was moved down.
		*/
		bool mouseWheelDown();

		/**
		*   @returns the x coordinate of the mouse.
		*/
		int getMousePositionX();

		/**
		*   @returns the y coordinate of the mouse.
		*/
		int getMousePositionY();
        
        /**
        *   @returns if the app was closed by the user. 
        */
        bool appClosed();
        
		private:

    };   
}


#endif	/* INPUT_H */

