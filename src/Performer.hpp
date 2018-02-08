#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include <unordered_map>
#include <set>
#include <cstring> //strncpy
#include <utility> //std::pair
#include "Messenger.hpp"
#include "Node.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "NeighbourManager.hpp"
#include "SearchNode.hpp"
#include "Updater.hpp"
#include "Logger.hpp"


class Performer
{
public:
    Performer(std::queue<Message*>* q);
    ~Performer();
    std::queue<Message*>* message_queue;
    std::unordered_map<Key, const char*> filesMap;
    std::unordered_map<Key, const std::string> storeTmpMap;
    std::unordered_map<Key, SearchNode> searchInProgress;
    NeighbourManager* neighbours;
    
    const pthread_t getThreadID()const;
    void printFilesMap();
    bool myselfHasValue(const Key* key);
    
private:
    pthread_t thread_id;
};

void rpc_ping(Node node);
void rpc_pong(Node node);
void rpc_store_request(const std::string& value, Performer* p);
void rpc_find_node(const Key* key, Performer* p);
void rpc_find_value(const Key* key, Performer* p);

Message generate_find_node_request(const Key* key);
Message generate_find_node_request(const Node findme);
Message generate_find_node_answer(const Key* key, Kbucket* bucket);

#endif
