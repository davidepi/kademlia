#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include <unordered_map>
#include "Messenger.hpp"
#include "Node.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "NeighbourManager.hpp"

class Performer
{
public:
    Performer(std::queue<Message*>* q);
    ~Performer();
    Node* myself;
    std::queue<Message*>* message_queue;
    std::unordered_map<std::string, char*> filesMap;
    
private:
    pthread_t thread_id;
    NeighbourManager* neighbours;
    
    void rpc_find_node(Node target_node, Node search_for, uint8_t iteration);
};


#endif
