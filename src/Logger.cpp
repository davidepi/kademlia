#include "Logger.hpp"

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    char name[42];
    char myIp[16];
    Messenger::getInstance().getIp().toString(myIp);
    uint16_t port = Messenger::getInstance().getPort();
    snprintf(name,42,"log_%s_%d_%d.log",myIp,port,(unsigned)time(NULL));
    Logger::fp = fopen(name, "w");
    if(fp == NULL)
    {
        fprintf(stderr,"%s%s\n","Couldn't create log file ",name);
        exit(EXIT_FAILURE);
    }
    Logger::log("--- Started peer ---");
}

Logger::~Logger()
{
    Logger::log("--- Ended peer ---");
    fclose(Logger::fp);
}


void Logger::log(const char* text)
{
    mtx.lock();
    char timestamp[16];
    snprintf(timestamp,167,"[%d] - ",(unsigned)time(NULL));
    fputs(timestamp,Logger::fp);
    fputs(text, Logger::fp);
    fputs("\n",Logger::fp);
    mtx.unlock();
}
