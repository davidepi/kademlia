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

#define LOG_NOTHING 0
#define LOG_STDOUT 1
#define LOG_FILE 2
#define LOG_EVERYWHERE 3

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
    
    /** \brief Set the log level
     *
     *  Set the amount of logging that will be performed by the logger class
     *  This is defined in the constants LOG_NOTHING, LOG_STDOUT, LOG_FILE and
     *  LOG_EVERYWHERE
     *
     *  \param[in] lvl The macro specifing the requested level of logging
     */
    void setLogLevel(int lvl);

    /** \brief Log to file
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
     *      t - Kbucket
     *
     */
    void logFile(const void* format...);
    
    /** \brief Log to stdout
     *
     *  Given a format, log formatted data to stdout. The format contains
     *  letters where every letter is a specifier. The following is the list
     *  of supported specifiers:
     *      s - ASCII null terminated c-string
     *      i - signed integer
     *      d - double
     *      b - boolean
     *      n - Node
     *      k - Key
     *      f - Message flag
     *      t - Kbucket
     *
     */
    void logStdout(const void* format...);
    
    ///Log both data to stdout and file
    void logBoth(const void* format...);

private:
    Logger():logLevel(LOG_NOTHING),fp(NULL){};
    ~Logger();
    //open the file descriptor for logFile
    void openFd();
    FILE* fp;
    std::mutex mtx;
    void writeToDisk(const char*);
    char logLevel;
};

#endif
