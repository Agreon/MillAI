/* 
 * File:   Sound.h
 * Author: daniel
 *
 * Created on February 11, 2015, 9:40 PM
 */

#ifndef SOUND_H
#define	SOUND_H

#include <SDL_mixer.h>

#include <map>

#include "Log.h"

namespace AL
{
    /**
    *	@brief Manages the sound.
    *
    *	You can either load a Sound or Music. Contains many functions to manage the sound.
    */
    class Sound
    {
    private:
            std::map< std::string, Mix_Chunk* > m_Chunks;
            std::map< std::string, Mix_Music* > m_Music;
    public:
            /**
            *	Initializes the Sound.
            */
            Sound();

            /**
            *	Deletes the data.
            */
            ~Sound();

            /**
            *	Loads a sound file in .wav format.
            *	
            *	@param	path	The path of the sound file.
            *	@param	name	The name of the sound.
            */
            bool loadSound( const char* path, std::string name );

            /**
            *	Loads a music file in .mp3 format.
            *	
            *	@param	path    The path of the music file.
            *	@param	name	The name of the music.
            */
            bool loadMusic( const char* path, std::string name );

            /**
            *	Plays a sound or music one time.
            *	
            *	@param name The name of the sound or music.
            */
            int playSound( std::string name );

            /**
            *   Plays a sound or music.
            *   
            *   @param name The name of the sound or music.
            *   @param loops    How often the Sound should be looped. -1 is infite loops.
            */
            int playSound(std::string name, short loops);
            
            /**
            *	Pauses a sound or music.
            *	
            *	@param name The name of the sound or music.
            */
            void pauseSound( std::string name );

            /**
            *	Resumes a sound or music.
            *	
            *	@param name The name of the sound or music.
            */
            void resumeSound( std::string name );

            /**
            *	Mutes a sound or music.
            *	
            *	@param name The name of the sound or music.
            */
            void muteSound( std::string name );

            /**
            *	Pauses all Sounds.
            */
            void pauseSounds();

            /**
            *	Resumes all Sounds.
            */
            void resumeSounds();

            /**
            *	Mutes all Sounds.
            */
            void muteSounds();

            /**
            *	Sets the volume of a sound or music.
            *   Range from 0-128.
            * 
            *	@param name The name of the sound or music.
            *	@param vol  The volume.
            */
            void setVolume( std::string name, int vol );

            /**
            *	Sets the main volume.
            *   Range from 0-128
            * 
            *	@param vol The volume.
            */
            void setVolume( int vol);

            /**
            *	@param name	The name of the sound.
            *	@returns        the sound object.
            */
            Mix_Music* getSound( std::string name );

            /**
            *	@param name	The name of the chunk.
            *	@returns        the chunk object.
            */
            Mix_Chunk* getChunk( std::string name );
    };
}

#endif	/* SOUND_H */

