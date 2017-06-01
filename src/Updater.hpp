#ifndef UPDATER_HPP__
#define	UPDATER_HPP__

#include <unistd.h>
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <queue>
#include <map>
#include "Node.hpp"
#include "Performer.hpp"

struct UpdaterStruct {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::queue<Node> queue;
    std::map<Node, Node> updateNodesMap;
};

class Updater {
public:
    static Updater* getInstance();
    ~Updater();
    void checkUpdateBucket(Node oldNode, Node newNode);
    void processPong(Node n);
    
private:
    Updater();
    static Updater* instance;
    pthread_t thread_id;
    struct UpdaterStruct* vars;
    
};

#endif

