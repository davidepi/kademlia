#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Messenger.hpp"
#include <cstdio>
#include <mutex>
#include <cstdarg>

class Logger
{
    
public:
    static Logger& getInstance();
    Logger(Logger const&)      = delete;
    void operator=(Logger const&) = delete;

    void logFormat(const void* format...);
    
    static const char* INCOMING;
    static const char* OUTGOING;
    
    static const char* UPDATER;
    static const char* PERFORMER;
    
private:
    Logger();
    ~Logger();
    FILE* fp;
    std::mutex mtx;
    void log(const char*);
};

#endif
