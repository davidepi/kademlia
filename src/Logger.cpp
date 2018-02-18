#include "Logger.hpp"
#include "Performer.hpp"

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(int lvl)
{
    Logger::logLevel = lvl;
}

Logger::~Logger()
{
    if(fp)
    {
        Logger::writeToDisk("--- Ended peer ---");
        fclose(Logger::fp);
    }
}

void Logger::openFd()
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
    char timestamp[16];
    snprintf(timestamp,167,"[%d] - ",(unsigned)time(NULL));
    fputs(timestamp,Logger::fp);
    fputs("--- Started peer ---", Logger::fp);
    fputs("\n",Logger::fp);
}

void Logger::writeToDisk(const char* text)
{
    mtx.lock();
    if(!fp)
        openFd();
    char timestamp[16];
    snprintf(timestamp,167,"[%d] - ",(unsigned)time(NULL));
    fputs(timestamp,Logger::fp);
    fputs(text, Logger::fp);
    fputs("\n",Logger::fp);
    mtx.unlock();
}

void Logger::logFile(const void *format...)
{
    if(logLevel==LOG_NOTHING||logLevel==LOG_STDOUT)
        return;
    std::stringstream ss;
    const char* fmt = (char*)format; //convert void* to char*
    va_list args;
    va_start(args, (char*)format);
    
    while (*fmt != '\0') {
        if (*fmt == 'i') {
            int i = va_arg(args, int);
            ss << i;
        } else if (*fmt == 'd') {
            double d = va_arg(args, double);
            ss << d;
        } else if (*fmt == 's') {
            char* s = va_arg(args, char*);
            ss << s;
        }else if (*fmt == 'b') {
            bool b = va_arg(args, int);
            ss << b;
        }else if (*fmt == 'n') {
            Node* n = va_arg(args, Node*);
            ss << *n;
        } else if (*fmt == 'k') {
            Key* k = va_arg(args, Key*);
            ss << *k;
        } else if (*fmt == 'f') {
            uint8_t f = *(va_arg(args, uint8_t*));
            
            if ((f & RPC_MASK) == RPC_PING)
                ss << "RPC_PING ";
            if ((f & RPC_MASK) == RPC_STORE)
                ss << "RPC_STORE ";
            if ((f & RPC_MASK) == RPC_FIND_NODE)
                ss << "RPC_FIND_NODE ";
            if((f & FLAG_ANSWER) > 0)
                ss << "ANSWER ";
            if((f & FLAG_FIND_VALUE) > 0)
                ss << "FIND_VALUE ";
            if ((f & FLAG_VALUE_FOUND) > 0)
                ss << "VALUE_FOUND ";
            if ((f & FLAG_STORE_REQUEST) > 0)
                ss << "STORE_REQUEST";
        }
        else if(*fmt == 't')
        {
            Kbucket* t = va_arg(args, Kbucket*);
            ss << *t;
        }
        ss << " ";
        fmt++;
    }
    
    va_end(args);
    writeToDisk(ss.str().c_str());
}

///////////////
//this is garbage, but looks like I cannot forward a ... list
///////////////


void Logger::logStdout(const void *format...)
{
    if(logLevel==LOG_NOTHING||logLevel==LOG_FILE)
        return;
    std::stringstream ss;
    const char* fmt = (char*)format; //convert void* to char*
    va_list args;
    va_start(args, (char*)format);
    
    while (*fmt != '\0') {
        if (*fmt == 'i') {
            int i = va_arg(args, int);
            ss << i;
        } else if (*fmt == 'd') {
            double d = va_arg(args, double);
            ss << d;
        } else if (*fmt == 's') {
            char* s = va_arg(args, char*);
            ss << s;
        }else if (*fmt == 'b') {
            bool b = va_arg(args, int);
            ss << b;
        }else if (*fmt == 'n') {
            Node* n = va_arg(args, Node*);
            ss << *n;
        } else if (*fmt == 'k') {
            Key* k = va_arg(args, Key*);
            ss << *k;
        } else if (*fmt == 'f') {
            uint8_t f = *(va_arg(args, uint8_t*));
            
            if ((f & RPC_MASK) == RPC_PING)
                ss << "RPC_PING ";
            if ((f & RPC_MASK) == RPC_STORE)
                ss << "RPC_STORE ";
            if ((f & RPC_MASK) == RPC_FIND_NODE)
                ss << "RPC_FIND_NODE ";
            if((f & FLAG_ANSWER) > 0)
                ss << "ANSWER ";
            if((f & FLAG_FIND_VALUE) > 0)
                ss << "FIND_VALUE ";
            if ((f & FLAG_VALUE_FOUND) > 0)
                ss << "VALUE_FOUND ";
            if ((f & FLAG_STORE_REQUEST) > 0)
                ss << "STORE_REQUEST";
        }
        else if(*fmt == 't')
        {
            Kbucket* t = va_arg(args, Kbucket*);
            ss << *t;
        }
        ss << " ";
        fmt++;
    }
    
    va_end(args);
    std::cout<<ss.str()<<std::endl;
}

void Logger::logBoth(const void *format...)
{
    if(logLevel==LOG_NOTHING)
        return;
    std::stringstream ss;
    const char* fmt = (char*)format; //convert void* to char*
    va_list args;
    va_start(args, (char*)format);
    
    while (*fmt != '\0') {
        if (*fmt == 'i') {
            int i = va_arg(args, int);
            ss << i;
        } else if (*fmt == 'd') {
            double d = va_arg(args, double);
            ss << d;
        } else if (*fmt == 's') {
            char* s = va_arg(args, char*);
            ss << s;
        }else if (*fmt == 'b') {
            bool b = va_arg(args, int);
            ss << b;
        }else if (*fmt == 'n') {
            Node* n = va_arg(args, Node*);
            ss << *n;
        } else if (*fmt == 'k') {
            Key* k = va_arg(args, Key*);
            ss << *k;
        } else if (*fmt == 'f') {
            uint8_t f = *(va_arg(args, uint8_t*));
            
            if ((f & RPC_MASK) == RPC_PING)
                ss << "RPC_PING ";
            if ((f & RPC_MASK) == RPC_STORE)
                ss << "RPC_STORE ";
            if ((f & RPC_MASK) == RPC_FIND_NODE)
                ss << "RPC_FIND_NODE ";
            if((f & FLAG_ANSWER) > 0)
                ss << "ANSWER ";
            if((f & FLAG_FIND_VALUE) > 0)
                ss << "FIND_VALUE ";
            if ((f & FLAG_VALUE_FOUND) > 0)
                ss << "VALUE_FOUND ";
            if ((f & FLAG_STORE_REQUEST) > 0)
                ss << "STORE_REQUEST";
        }
        else if(*fmt == 't')
        {
            Kbucket* t = va_arg(args, Kbucket*);
            ss << *t;
        }
        ss << " ";
        fmt++;
    }
    
    va_end(args);
    if(logLevel==LOG_FILE)
        writeToDisk(ss.str().c_str());
    if(logLevel==LOG_STDOUT)
        std::cout<<ss.str()<<std::endl;
}
