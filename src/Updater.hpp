#ifndef UPDATER_HPP__
#define	UPDATER_HPP__

#include <unistd.h>
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <queue>
#include <map>
#include <list>
#include "Node.hpp"
#include "Performer.hpp"
#include "Kbucket.hpp"
#include "settings.h"

struct UpdaterStruct;

class Updater {
public:
    static Updater* getInstance();
    ~Updater();
    void checkUpdateBucket(Node oldNode, Node newNode, Kbucket* kbucket);
    void processPong(Node n);   
    
private:
    Updater();
    static Updater* instance;
    pthread_t thread_id;
    struct UpdaterStruct* vars;
    
};

#endif

