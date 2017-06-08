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

void rpc_ping(Node node);

class Performer
{
public:
    Performer(std::queue<Message*>* q);
    ~Performer();
    std::queue<Message*>* message_queue;
    std::unordered_map<std::string, char*> filesMap;
    NeighbourManager* neighbours;
    
    void rpc_find_node_request(Node ask_me, Node find_me, uint8_t iteration);
    void rpc_find_node_answer(Node target, Node findme, Kbucket* bucket);
    const pthread_t getThreadID()const;
    
private:
    pthread_t thread_id;
};

#endif
