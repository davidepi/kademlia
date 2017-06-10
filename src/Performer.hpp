#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include <unordered_map>
#include <cstring> //strncpy
#include "Messenger.hpp"
#include "Node.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "NeighbourManager.hpp"
#include "SearchNode.hpp"

void rpc_ping(Node node);
Message generate_find_node_request(Node findme);
Message generate_find_node_answer(Node findme, Kbucket* bucket);

class Performer
{
public:
    Performer(std::queue<Message*>* q);
    ~Performer();
    std::queue<Message*>* message_queue;
    std::unordered_map<std::string, char*> filesMap;
    std::unordered_map<const Key*,SearchNode*> searchInProgress;
    NeighbourManager* neighbours;
    
    const pthread_t getThreadID()const;
    
private:
    pthread_t thread_id;
};

#endif
