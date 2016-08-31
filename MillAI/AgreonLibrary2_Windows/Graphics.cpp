/* 
 * @file:   Graphics.cpp
 * @author: daniel
 *  @brief This file contains the Graphics functions.
 * 
 * Created on February 2, 2015, 5:12 PM
 */

#include "Graphics.h"

namespace AL
{
    Graphics::Graphics(void)
    {
        m_Screen_Width = 0;
        m_Screen_Height = 0;
        
    }

    Graphics::~Graphics(void)
    {
    }

    void Graphics::init(int screen_width, int screen_height, const char* windowTitle, const char* iconSrc)
    {
            Log::reset();
            m_Screen_Width  = screen_width;
            m_Screen_Height = screen_height;

            SDL_Init(SDL_INIT_EVERYTHING);

            if(iconSrc != NULL)
            {
                SDL_Surface* icon = SDL_LoadBMP(iconSrc);
                SDL_SetWindowIcon(m_Window, icon);
            }
            m_Window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Screen_Width, m_Screen_Height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
            
            if(m_Window == NULL)
            {
				Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Error while creating window: ") + SDL_GetError());
            }
            
            if(windowTitle != NULL)
            {
                SDL_SetWindowTitle(m_Window, windowTitle);
            }

            initOpenGL();
            
			Log::write(AL::LOG_INFORMATION, "GraphicsManager", "Initialized Window and OpenGL.");
    }

    void Graphics::initOpenGL()
    {        
        m_GLcontext = SDL_GL_CreateContext(m_Window);
                
        glViewport(0, 0 ,m_Screen_Width, m_Screen_Height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_Screen_Width, m_Screen_Height, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.5, 0.5, 0.5, 1.0);

        glShadeModel(GL_SMOOTH);
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClearDepth( 0.0f);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glDisable(GL_LIGHTING);

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_SCISSOR_TEST);
        
    }


    bool Graphics::loadTexture(const char* path, std::string name)
    {
        uint id = 0;
        SDL_Surface* img = IMG_Load( path );
        if( img == NULL )
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("ERROR: Texture was not loaded! Path: ") + path);
            glDisable( GL_TEXTURE_2D );
            return false;
        }
 
        glGenTextures( 1, &id );
        glBindTexture( GL_TEXTURE_2D, id );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img->w,img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels );
        SDL_FreeSurface(img);
        
        m_Textures[ name ] = id;
        
        std::string msg = "Loaded texture successfully. "+name+" | "+path;       
		Log::write(AL::LOG_INFORMATION, "GraphicsManager", msg);
        
        return true;
    }


    bool Graphics::drawTexture( std::string ID, int x, int y )
    {
        return drawTexture( ID, x, y, 0, 1 );
    }


    bool Graphics::drawTexture( std::string ID, int x, int y, int rotation )
    {
        return drawTexture( ID, x, y, rotation, 1 );
    }


    bool Graphics::drawTexture( std::string ID, int x, int y, int rotation, double scale )
    {
        int w;
        int h;

        if( m_Textures[ ID ] == NULL )
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Can't draw texture with unkown ID! ID: ") + ID);
            return false;
        }

        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, m_Textures[ ID ] );

        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h );

        glPushMatrix();

        glScalef( scale, scale , 0 );

            /// Rotation
        glTranslatef(x + w/2, y + h/2, 0);
        glRotatef( rotation, 0, 0, 1 );
        glTranslatef(-x - w/2, -y - h/2, 0);

        glBegin( GL_QUADS );
            glTexCoord2f(0,0); glVertex2f(x,y);
            glTexCoord2f(1,0); glVertex2f(x+w,y);
            glTexCoord2f(1,1); glVertex2f(x+w,y+h);
            glTexCoord2f(0,1); glVertex2f(x,y+h);
        glEnd();
        glDisable( GL_TEXTURE_2D );

        glPopMatrix();

        return true;
    }


    void Graphics::drawSection( std::string texture, int rectX, int rectY, int rectW, int rectH, int posX, int posY, int rotation )
    {
        uint id= getUint( texture );
        int textureWidth;
        int textureHeight;

        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, id );


        // Get width and height of the texture
        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight );


        double texLeft = (double)rectX / (double)textureWidth;
        double texRight = ((double)rectX + (double)rectW) / (double)textureWidth;
        double texTop = (double)rectY / (double)textureHeight;
        double texBottom = ((double)rectY + (double)rectH) / (double)textureHeight;

        glPushMatrix();

        // Rotation
        glTranslatef(rectX + rectW/2,rectY + rectH/2,0);
        glRotatef(rotation,0,0,1);
        glTranslatef(-rectX - rectW/2,-rectY - rectH/2,0);

        glBegin( GL_QUADS );
            glTexCoord2f(texLeft,texTop); glVertex2f( posX, posY );
            glTexCoord2f(texRight,texTop); glVertex2f( posX+rectW, posY );
            glTexCoord2f(texRight,texBottom); glVertex2f( posX+rectW, posY+rectH );
        glTexCoord2f(texLeft,texBottom); glVertex2f( posX, posY+rectH );
        glEnd();
        glDisable( GL_TEXTURE_2D );

        glPopMatrix();
        //glLoadIdentity();

        glDisable( GL_TEXTURE_2D );
    }

    /*
    void cGraphics::drawSection( uint texture, int section, int x, int y, int w, int h, int rotation )
    {
            int textureWidth;
            int textureHeight;

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,texture);

            // Get width and height of the texture
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);

            Rect clip;
            clip.x = section * w;
            clip.y = 0;
            clip.w = w;
            clip.h = h;

            double texLeft = 0;
            double texRight = 1;
            double texTop = 0;
            double texBottom = 1;

            texLeft = (double)clip.x / (double)textureWidth;
            texRight = (double)(clip.x + clip.w) / (double)textureWidth;
            texTop = (double)clip.y / (double)textureWidth;
            texBottom = 1;

            glPushMatrix();

            // Rotation
            glTranslatef(x + w/2,y + h/2,0);
            glRotatef(rotation,0,0,1);
            glTranslatef(-x - w/2,-y - h/2,0);

            glBegin(GL_QUADS);
                    glTexCoord2f(texLeft,texTop); glVertex2f(x,y);
                    glTexCoord2f(texRight,texTop); glVertex2f(x+w,y);
                    glTexCoord2f(texRight,texBottom); glVertex2f(x+w,y+h);
            glTexCoord2f(texLeft,texBottom); glVertex2f(x,y+h);
            glEnd();
            glDisable(GL_TEXTURE_2D);

            glPopMatrix();
            glLoadIdentity();
    }
    */
    /**
    *	Draws a section of a texture. Used mostly for Animation
    *	
    *	@see above	But with the name of the texture. 

    void cGraphics::drawSection( string texture, int section, int x, int y, int w, int h, int rotation )
    {
            uint id = getUint( texture );

            drawSection( id, section, x, y, w, h, rotation );
    }
    */

    void Graphics::drawRectangle( int x, int y, int w, int h )
    {
        glBegin( GL_QUADS );
                glVertex2f( x, y );
                glVertex2f( x + w, y );
                glVertex2f( x + w, y + h );
                glVertex2f( x, y + h );
        glEnd();
    }


    void Graphics::setColor( color c )
    {
        glColor4d( c.r, c.g, c.b, c.a );
    }


    void Graphics::bindTexture( std::string ID )
    {
        glBindTexture( GL_TEXTURE_2D, m_Textures[ID] );
    }


    uint Graphics::getUint( std::string ID )
    {
        if(m_Textures[ID] == NULL)
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Can not get UINT of texture with unkown ID! ID: ") + ID);
            return m_Textures[0];
        }
        return m_Textures[ID];
    }


    int Graphics::getTextureWidth( std::string ID )
    {
        if(m_Textures[ID] == NULL)
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Can not get width of texture with unkown ID! ID: ") + ID);
            return m_Textures[0];
        }

        int width;

        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, m_Textures[ID] );

        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );

        glDisable(GL_TEXTURE_2D);

        return width;
    }


    int Graphics::getTextureHeight( std::string ID )
    {
        if(m_Textures[ID] == NULL)
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Can not get height of texture with unkown ID! ID: ") + ID);
            return m_Textures[0];
        }

        int height;

        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, m_Textures[ID] );

        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );

        glDisable(GL_TEXTURE_2D);

        return height;
    }


    void Graphics::drawText( const char* text, int x, int y )
    {
        if(m_CurrentFont != NULL)
        {
            glRasterPos2i( x, y );
            m_CurrentFont->Render( text );
        }
        else
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", "There is no font set. Please use 'setFont' to select a font you loaded!");
            shutdown();
        }
    }


    void Graphics::drawNumber( int num, int x, int y )
    {
		char buf[33];
		int tmp = num;
		_itoa_s(tmp, buf, 10);

		drawText(buf, x, y);
    }


    void Graphics::setFont(std::string name, ushort fontSize, color fontColor)
    {
        std::map<std::string, FTGLPixmapFont*>::iterator iter = m_Fonts.find( name );
        if( iter != m_Fonts.end() )
        {
            m_CurrentFont = m_Fonts[ name ];
            m_CurrentFont->FaceSize(fontSize);
            setFontColor(fontColor);
        }
        else
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("There is no font loaded with this name! Name: ") + name);
        }
    }


    bool Graphics::loadFont( const char* font, std::string name )
    {
        m_Fonts[ name ] = new FTGLPixmapFont( font );

        if(m_Fonts[ name ]->Error()) 
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", std::string("Font was not loaded ! Path: ") + font);
            return false;
        }
        
		Log::write(AL::LOG_INFORMATION, "GraphicsManager", std::string("Loaded font file successful. ") + name + " | " + font);
        return true;
    }


    void Graphics::setFontColor( color c )
    {
        if( c.r > 0 ) c.r = -1 + c.r;
        else c.r = -1;

        if( c.g > 0 ) c.g = -1 + c.g;
        else c.g = -1;

        if( c.b > 0 ) c.b = -1 + c.b;
        else c.b = -1;
        
        if( c.a > 0 ) c.a = -1 + c.a;
        else c.a = -1;

        glPixelTransferf( GL_RED_BIAS, c.r );
        glPixelTransferf( GL_GREEN_BIAS, c.g );
        glPixelTransferf( GL_BLUE_BIAS, c.b );
        glPixelTransferf( GL_ALPHA_BIAS, c.a );
    }

    void Graphics::setFontSize( int size )
    {
        if(m_CurrentFont != NULL)
        {
            m_CurrentFont->FaceSize( size );
        }
        else
        {
			Log::write(AL::LOG_ERROR, "GraphicsManager", "ERROR: There is no font set. Please use 'setFont' to select a font you loaded!");
            shutdown();
        }
    }

    void Graphics::swapWindow()
    {
        SDL_GL_SwapWindow(m_Window);
    }

    void Graphics::shutdown()
    {
        SDL_GL_DeleteContext(m_GLcontext);
        SDL_DestroyWindow(m_Window);
        
        delete m_CurrentFont;
        m_Fonts.clear();
        
        SDL_Quit();
    }
}
