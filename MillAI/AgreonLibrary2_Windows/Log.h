/* 
 * File:   Log.h
 * Author: daniel
 *
 * Created on February 2, 2015, 6:27 PM
 */

#ifndef LOG_H
#define	LOG_H

#include <iostream>
#include <fstream>
#include <time.h>

namespace AL
{
	enum
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_INFORMATION
	};

    class Log
    {
    private:
    public:
            static void reset()
            {
                std::ofstream stream;
                stream.open("log.txt");
                stream.close();
            }
            
            static void write(int log_level, const std::string& sender, const std::string& text)
            {
                std::ofstream stream;
               /* if(m_Filename == "")
                {
                    m_Filename = "log.txt";
                    stream.open(m_Filename);
                    stream.close();
                }*/

                stream.open("log.txt", std::ios::out | std::ios::app);

                if(!stream.is_open())
                {
                    return;
                }	

                time_t t = time(0);  
                struct tm * now = localtime(&t);
				/*time_t timev;
				std::time(&timev);*/

                stream << "[";

                if( now->tm_hour < 10 )
                {
                    stream << "0";
                }
                stream << now->tm_hour << ":";

                if( now->tm_min < 10 )
                {
                    stream << "0";
                }
                stream << now->tm_min << ":";

                if( now->tm_sec < 10 )
                {
                    stream << "0";
                }
                stream << now->tm_sec << "] ";

				if (log_level == LOG_ERROR) stream << "[ERROR] ";
				else if (log_level == LOG_WARNING) stream << "[WARNING] ";
				else if (log_level == LOG_INFORMATION) stream << "[INFO] ";

				stream << "'" << sender.c_str() << "': " << text.c_str() << std::endl;
                
                stream.close();
            }
    };
    
    /**
     * TODO log
     
    class Log
    {
    private:
        static std::ofstream m_Stream;
        static std::string m_Filename;
    public:
            /**
            *	Adds a Timestamp and writes something to the log file. 
            *
            *	@param text The text to write to the log file.
            
            //static void write(std::string text);
            static void write(std::string text)
            {
                /*if(m_Filename == NULL)
                {
                    m_Filename = "log.txt";
                }

                m_Stream.open("log.txt", std::ios::out | std::ios::app);

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
            */
            
            /**
            *	Adds a Timestamp and writes something to the log file. 
            *
            *	@param text The text to write to the log file.
            */
           // static void operator<<(std::string text);
            
            //static void setFilename(std::string filename);
    //};
}


#endif	/* LOG_H */

