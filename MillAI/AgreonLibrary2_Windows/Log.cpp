/* 
 * File:   Log.cpp
 * Author: daniel
 * 
 * Created on February 2, 2015, 6:27 PM
 */

#include "Log.h"
/*
namespace AL
{
    static void Log::write(std::string text)
    {
        if(m_Filename == NULL)
        {
            m_Filename = "log.txt";
        }

        m_Stream.open(m_Filename, std::ios::out | std::ios::app);
        
        if(!m_Stream.is_open())
        {
            return;
        }	
       
        time_t t = time(0);  
        struct tm * now = localtime(&t);
        m_Stream << "[ ";

        if( now->tm_hour < 10 )
        {
            m_Stream << "0";
        }
        m_Stream << now->tm_hour << ":";

        if( now->tm_min < 10 )
        {
            m_Stream << "0";
        }
        m_Stream << now->tm_min << ":";

        if( now->tm_sec < 10 )
        {
            m_Stream << "0";
        }
        m_Stream << now->tm_sec << " ] ";

        m_Stream << text << std::endl;
        m_Stream.close();
    }

    static void Log::operator<<( std::string text )
    {
            write(text);
    }
    
    static void Log::setFilename(std::string filename)
    {
        m_Filename = filename;
    }
}
*/


