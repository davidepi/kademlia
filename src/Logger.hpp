#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <cstdio>
#include <mutex>
#include <cstdarg>

#define LOGGER_INCOMING "[INCOMING]"
#define LOGGER_OUTGOING "[OUTGOING]"
#define LOGGER_UPDATER "[Update Bucket]"
#define LOGGER_PERFORMER "[Performer]"
#define LOGGER_KBUCKET "[KBucket]"
#define LOGGER_SEARCHNODE "[SearchNode]"

/** \brief Class used to log events to a file
 *
 *  The logger class is a singleton used to log relevant events that happened
 *  during the execution of this host. The file is created as IP_PORT_TIMESTAMP
 *  in the working folder
 */
class Logger
{
public:
    ///Get the instance of this singleton
    static Logger& getInstance();
    ///Ensure only one instance
    Logger(Logger const&)      = delete;
    ///Ensure only one instance
    void operator=(Logger const&) = delete;

    /** \brief Log
     *
     *  Given a format, log formatted data to the file. The format contains
     *  letters where every letter is a specifier. The following is the list
     *  of supported specifiers:
     *      s - ASCII null terminated c-string
     *      i - signed integer
     *      d - double
     *      b - boolean
     *      n - Node
     *      k - Key
     *      f - Message flag
     *
     */
    void logFormat(const void* format...);

private:
    Logger();
    ~Logger();
    FILE* fp;
    std::mutex mtx;
    void log(const char*);
};

#endif
