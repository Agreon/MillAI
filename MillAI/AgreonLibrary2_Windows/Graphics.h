/* 
 * File:   Graphics.h
 * Author: daniel
 *
 * Created on February 2, 2015, 5:12 PM
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <ft2build.h> 
#include FT_FREETYPE_H
#include <FTGL/ftgl.h>

#include <map>

#include "Log.h"

namespace AL
{ 
    typedef unsigned int uint;
    typedef unsigned short ushort;

    struct color
    {
        double r,g,b,a;
        
        /**
        *   Constructor. Use values between 0 and 1. Sets the alpha value to 1.
        * 
        *   @param	r The red color.
	*   @param	g The green color.
	*   @param	b The blue color.
         */
        color(double _r, double _g, double _b)
        : r(_r),
        g(_g),
        b(_b),
        a(1){}
        
        /**
        *   Constructor. Use values between 0 and 1. 
        * 
        *   @param	r The red color.
	*   @param	g The green color.
	*   @param	b The blue color.
	*   @param	a The alpha value.
         */
        color(double _r, double _g, double _b, double _a)
        : r(_r),
        g(_g),
        b(_b),
        a(_a){}
    };
    
    /** 
    *	@brief Cares about OpenGL initialization, window handling and drawing.
    *
    *	This is a main class. Without it you can't run other managers, because they need openGL. 
    *
    *	You need this class to open a window, draw textures and to draw text or numbers.
    *
    *	Source : Graphics.h
    *
    *	Source : Graphics.cpp
    *
    */
    class Graphics 
    {
    private:
        SDL_Window* m_Window;
        int         m_Screen_Width;
        int         m_Screen_Height;
        
        SDL_GLContext m_GLcontext;
        
        FTGLPixmapFont* m_CurrentFont;      
        
        /**
         * Contains all textures, with their identifiers. 
         */
        std::map<std::string, uint> m_Textures;      
        std::map<std::string, FTGLPixmapFont*> m_Fonts;        
    public:
        Graphics();
        ~Graphics();
        
        /**
	*   Creates a window and initializes OpenGL.
	*
	*   @param screen_width     Width of the window.
	*   @param screen_height    Height of the window.
	*   @param windowTitle      Title of the window.
	*   @param iconSrc          Source of the window icon.
	*/
	void init(int screen_width, int screen_height, const char* windowTitle, const char* iconSrc);
        
        /**
	*   Loads a texture from a given path and stores it in a map.
	*
	*   @param path	Path of the texture.
	*   @param name Name of the texture.
	*   @returns If the texture was successfully loaded.
	*/
	bool loadTexture(const char* path, std::string name);
        
        /**
	*   Draws a texture on the screen.
	*
	*   @param ID   The name of the texture.
	*   @param x    The x coordinate on the screen.
	*   @param y    The y coordinate on the screen.
	*/
	bool drawTexture(std::string ID, int x, int y);
	
        /**
	*   Draws a texture on the screen.
	*
	*   @param ID       The name of the texture.
	*   @param x        The x coordinate on the screen.
	*   @param y        The y coordinate on the screen.
	*   @param rotation The rotation of the texture.
	*/
	bool drawTexture(std::string ID, int x, int y, int rotation);

	/**
	*   Draws a texture on the screen.
	*
	*   @param ID       The name of the texture.
	*   @param x        The x coordinate on the screen.
	*   @param y        The y coordinate on the screen.
	*   @param rotation The rotation of the texture.
	*   @param scale    The scale of the texture.
	*/
	bool drawTexture(std::string ID, int x, int y, int rotation, double scale);

	/**
	*   Draws a section of a texture.
	*	
	*   @param ID       The name of the texture.
	*   @param rectX    The x coordinate of the source rectangle.
	*   @param rectY    The y coordinate of the source rectangle.
	*   @param rectW    The width of the source rectangel.
	*   @param rectH    The height of the source rectangel.
	*   @param posX     The x coordinate on the screen.
	*   @param posY     The y coordinate on the screen.
	*   @param rotation The rotation of the texture.
	*/
	void drawSection(std::string ID, int rectX, int rectY, int rectW, int rectH, int posX, int posY, int rotation);

	/**
	*   Draws a rectangle on the screen.
	*
	*   @param x    The x coordinate of the Rectangle.
	*   @param y	The y coordinate of the Rectangle.
	*   @param w	The width of the Rectangle.
	*   @param h	The width of the Rectangle.
	*/
	void drawRectangle(int x, int y, int w, int h);

	/**
	*   Sets the color that will modify the textures.
	*
	*   @param c    The color.
	*/
	void setColor(color c);

	/**
	*   Selects a texture to work with.
	*   The light version of glBindTexture.
	*	
	*   @param ID   The name of the texture.
	*/
	void bindTexture(std::string ID);

	/**
	*   Returns the system ID of a texture.
	*		
	*   @param ID   The name of the texture.
	*   @returns    The system ID of the texture.
	*/
	uint getUint(std::string ID);

	/**
	*   Returns the width of a texture.
	*	
	*   @param ID   The name of the texture.
	*   @returns    The width of the texture.
	*/
	int getTextureWidth(std::string ID);

	/**
	*   Returns the height of a texture.
	*	
	*   @param ID   The name of the texture.
	*   @returns    The height of the texture.
	*/
	int getTextureHeight(std::string ID);

	/**
	*   Draws a text on the screen.
	*	
	*   @param text The text.
	*   @param x    The x coordinate.
	*   @param y	The y coordinate.
	*/
	void drawText(const char* text, int x, int y);

	/**
	*   Draws a number on the screen.
	*	
	*   @param num  The number.
	*   @param x    The x coordinate.
	*   @param y    The y coordinate.
	*/
	void drawNumber(int num, int x, int y);

	/**
	*   Sets the font face.
	*	
	*   @param name      The name of the font.
        *   @param fontSize  The initial size of the font.
        *   @param fontColor The initial color of the font.
	*/
	void setFont(std::string name, ushort fontSize, color fontColor);

	/**
	*   Loads a fontfile from a given path and stores it in a map.
	*
	*   @param path The Path of the fontfile.
	*   @param name The Naame of the font.
	*   @returns    If the texture was successfully loaded.
	*/
	bool loadFont(const char* font, std::string name);

	/**
	*   Sets the font color.
	*	
	*   @param c    The color of the font.
	*/
	void setFontColor(color c);

	/**
	*   Sets the size of the font.
	*	
	*   @param size The size of the font.
	*/
	void setFontSize(int size);

        /**
         * Swaps the Screen.
         */
        void swapWindow();
        
	/**
	*   Closes the window and deletes the pointers.
	*/
	void shutdown();
        
    private:
           
        /**
         *  Initializes OpenGL with default 2D options.
         */
        void initOpenGL();
        
    };
}


#endif	/* GRAPHICS_H */

