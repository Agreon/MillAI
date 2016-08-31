/* 
 * File:   Sound.cpp
 * Author: daniel
 * 
 * Created on February 11, 2015, 9:40 PM
 */

#include "Sound.h"

namespace AL
{ 
    Sound::Sound()
    {
		int audio_rate = 44800;
		Uint16 audio_format = MIX_DEFAULT_FORMAT; 
		int audio_channels = 2;
		int audio_buffers = 2048;

		Mix_OpenAudio( audio_rate, audio_format, audio_channels, audio_buffers );

        Log::write(AL::LOG_INFORMATION,"SoundManager","Initialized Sound.");
    }


    Sound::~Sound()
    {
		Log::write(AL::LOG_INFORMATION, "SoundManager", "Shutting down Sound...");
		m_Chunks.clear();
		m_Music.clear();
		Mix_CloseAudio();
    }

    bool Sound::loadSound(const char* path, std::string name)
    {
		Mix_Chunk* tmp = Mix_LoadWAV(path);
		if(tmp == NULL) 
		{
			Log::write(AL::LOG_ERROR, "SoundManager", std::string("ERROR: Could not load Sound file! Path: ") + path);
			return false; 
		}

		m_Chunks[name] = tmp;
		Log::write(AL::LOG_INFORMATION, "SoundManager", std::string("Loaded Sound file successful. ") + name + " | " + path);
		return true;
    }

    bool Sound::loadMusic(const char* path, std::string name)
    {
		Mix_Music* tmp = Mix_LoadMUS(path);
		if(tmp == NULL) 
		{
			Log::write(AL::LOG_ERROR, "SoundManager", std::string("ERROR: Could not load Music file! Path: ") + path);
			  return false;
		}
		m_Music[name] = tmp;
		Log::write(AL::LOG_INFORMATION, "SoundManager", std::string("Loaded Music file successful. ") + name + " | " + path);
		return true;
    }


    int Sound::playSound(std::string name)
    {
        return playSound(name,0);
    }
 
    
    int Sound::playSound(std::string name, short loop)
    {
        std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);
        
		if (iterChunk != m_Chunks.end())
		{
			return Mix_PlayChannel(-1,m_Chunks[name], loop);
		}
		else if(iterMusic != m_Music.end())
		{
			if(loop >= 0) loop++;
			Mix_PlayMusic(m_Music[name], loop);
			return 0;
		}
		else
		{
			Log::write(AL::LOG_ERROR, "SoundManager", std::string("ERROR: Could not play Sound with unkown ID! Name: ") + name);
			return 0;
		}
    }
    
    
    void Sound::pauseSound(std::string name)
    {
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);
		std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);

		if (iterChunk != m_Chunks.end())
		{
				Mix_HaltChannel(-1);
		}
		else if(iterMusic != m_Music.end())
		{
				Mix_PauseMusic();
		}
    }


    void Sound::resumeSound(std::string name)
    {
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);
		std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);

		if (iterChunk != m_Chunks.end())
		{
				Mix_Resume(1);
		}
		else if(iterMusic != m_Music.end())
		{
				Mix_ResumeMusic();
		}
    }

    void Sound::muteSound(std::string name)
    {
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);
		std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);

		if (iterChunk != m_Chunks.end())
		{
				if(Mix_Volume(1,-1) == 0) Mix_Volume(1,128);
				else Mix_Volume(1,0);
		}
		else if(iterMusic != m_Music.end())
		{
				Mix_VolumeMusic(0);
		}
    }


    void Sound::setVolume( std::string name, int vol )
    {
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);
		std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);

		if (iterChunk != m_Chunks.end())
		{
				Mix_VolumeChunk(m_Chunks[name],vol);
		}
		else if(iterMusic != m_Music.end())
		{
				// TODO: Seems not to function with music
		}
    }


    void Sound::setVolume(int vol)
    {
		Mix_VolumeMusic(vol);
		Mix_Volume( -1, vol );
    }


    void Sound::pauseSounds()
    {
        Mix_Pause(-1);
    }

    void Sound::resumeSounds()
    {
		Mix_Resume(-1);
    }

    void Sound::muteSounds()
    {
		if(Mix_Volume(-1,-1) == 0) Mix_Volume(-1,128);
		else Mix_Volume(-1,0);
    }


    Mix_Music* Sound::getSound( std::string name )
    {
		std::map<std::string, Mix_Music*>::iterator iterMusic = m_Music.find(name);

		if(iterMusic != m_Music.end())
		{
			return m_Music[ name ];
		}
		return 0;
    }

    Mix_Chunk* Sound::getChunk( std::string name )
    {
		std::map<std::string, Mix_Chunk*>::iterator iterChunk = m_Chunks.find(name);

		if(iterChunk != m_Chunks.end())
		{
			   return m_Chunks[ name ];
		}
		return 0;
    }
}


