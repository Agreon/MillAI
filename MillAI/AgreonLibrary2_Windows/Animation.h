/* 
 * File:   Animation.h
 * Author: daniel
 *
 * Created on February 17, 2015, 12:22 AM
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#include "Graphics.h"

namespace AL
{

    /** 
    *	@brief Holds an Animation and handles the operations on it.
    *
    *	You should not use this class to create Animations. Use AnimationManager instead.
    *
    *	Source : Animation.h
    *
    *	Source : Animation.cpp
    */
    class Animation
    {
    private:
            std::string m_Texture;
            uint m_Currentframe;
            uint m_Frames;
            Uint32 m_Lastupdate;
            Uint32 m_Fps;

            bool m_IsDone, m_IsPaused, m_Active;

            int m_X, m_Y, m_FrameWidth, m_FrameHeight, m_Repeatations, m_Rotation;
    public:
            Animation();
            Animation(int repeat, int fps, int rotation, int x, int y, int fw, int fh, bool active);

            void setTexture(const std::string texture, uint ID );

            void update();
            void show(Graphics* Graphics);
            void stop();
            void pause();
            void resume();
            void activate();
            void deactivate();

            bool isDone();
            bool isPaused();
            bool isActive();

            void setX(int y);
            void setY(int y);
            void setXY(int x, int y);
            void setRotation(int rotation);
            void setActive(bool active);

            int getRotation();
            std::string getTexture();
    };

    /** 
    *	@brief Stores all Animations and manages operations on them.
    *
    *	Use this class to easy create Animations.
    *
    *	Source : Animation.h
    *
    *	Source : Animation.cpp
    *
    */
    class AnimationManager
    {
    private:
            std::map<const std::string, Animation*> m_Animations;
            Graphics* m_Graphics;
    public:
            /**
            *	Constructor, this manager needs cGraphics to draw the Animations.
            */
            AnimationManager(Graphics* Graphics);

            /**
            *	Creates a new animation.
            *	@param	name		The name of the animation.
            *	@param	texture		The name of the texture to use.
            *	@param	repeats		The repeatations of the animation. -1 is endless.
            *	@param	speed		The speed of the animation
            *	@param	rotation	The rotation of the animation.
            *	@param	x		The x coordinate of the animation.
            *	@param	y		The y coordinate of the animation.
            *	@param	fw		The width of the rectangle that should be shown.
            *	@param	fh		The height of the rectangle that should be shown.
            */
            void createAnimation(const std::string name, const std::string texture, int repeats, int speed, int rotation, int x, int y, int fw, int fh );

            /**
            *	Updates all animations.
            */
            void updateAll();	

            /**
            *	Shows all animations.
            */
            void showAll(); 

            /**
            *	Stops all animations.
            */
            void stopAll();

            /**
            *	Pauses all animations.
            */
            void pauseAll();

            /**
            *	Resumes all animations.
            */
            void resumeAll();

            /**
            *	Deletes all animations.
            */
            void shutdown();

            /**
            *	Updates an animation.
            *
            *   @param ID   ID of the animation.
            */
            void update(const std::string ID);	

            /**
            *	Shows an animation.
            *
            *   @param ID   ID of the animation.
            */
            void show(const std::string ID);

            /**
            *	Stops an animation.
            *
            *   @param ID   ID of the animation.
            */
            void stop(const std::string ID);

            /**
            *	Pauses an animation.
            *
            *   @param ID   ID of the animation.
            */
            void pause(const std::string ID);

            /**
            *	Resumes an animation.
            *
            *   @param ID   ID of the animation.
            */
            void resume(const std::string ID);

            /**
            *	Destroys an animation.
            *
            *   @param ID   ID of the animation.
            */
            void destroy(const std::string ID);

            /**
            *	Activates an animation.
            *
            *   @param ID   ID of the animation.
            */
            void activate(const std::string ID);

            /**
            *	Deactivates an animation.
            *
            *   @param ID   ID of the animation.
            */
            void deactivate(const std::string ID);

            /**
            *	Sets the x coordinate of an animation.
            *
            *   @param ID   ID of the animation.
            *   @param x    The new x position.
            */
            void setX(const std::string ID, int x);

            /**
            *	Sets the y coordinate of an animation.
            *
            *   @param ID   ID of the animation.
            *   @param y    The new y position.
            */
            void setY(const std::string ID, int y);

            /**
            *	Sets the position of an animation.
            *
            *   @param ID   ID of the animation.
            *   @param x    The new x position.
            *   @param y    The new y position.
            */
            void setPosition(const std::string ID, int x, int y);

            /**
            *	Sets the rotation of an animation.
            *
            *   @param ID       ID of the animation.
            *   @param rotation The new rotation.
            */
            void setRotation(const std::string ID, int rotation);

            /**
            *	Sets the texture of an animation.
            *
            *   @param ID       ID of the animation.
            *   @param texture  The new texture.
            */
            void setTexture( const std::string ID, const std::string texture );

            /**
            *	@returns the rotation of an animation.
            *
            *   @param ID   ID of the animation.
            */
            int getRotation(const std::string ID);
    };
}

#endif	/* ANIMATION_H */

