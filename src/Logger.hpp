#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "Messenger.hpp"
#include <cstdio>
#include <mutex>

class Logger
{
    
public:
    static Logger& getInstance();
    Logger(Logger const&)      = delete;
    void operator=(Logger const&) = delete;
    void log(const char*);
    
private:
    Logger();
    ~Logger();
    FILE* fp;
    std::mutex mtx;
};

#endif
