#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include <unordered_map>
#include <set>
#include <cstring> //strncpy
#include <utility> //std::pair
#include <pthread.h>
#include <signal.h>
#include "Messenger.hpp"
#include "Node.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "NeighbourManager.hpp"
#include "SearchNode.hpp"
#include "Updater.hpp"
#include "Logger.hpp"

#define RPC_PING 0x1
#define RPC_STORE 0x2
#define RPC_FIND_NODE 0x3
#define RPC_MASK 0x3 //store or find or ping

#define FLAG_ANSWER 0x4 //the received message is an answer
#define FLAG_FIND_VALUE 0x8
#define FLAG_VALUE_FOUND 0x10
#define FLAG_STORE_REQUEST 0x20

/** \brief Class used to handle the various RPCs
 *
 *  Performer class is the core of the entire program. The class creates a
 *  thread responsible of handling the various incoming messages. After
 *  receiving a message it performs some actions, and then answer appropriately
 *  to the message
 */
class Performer
{
public:

    ///Default constructor
    Performer(std::queue<Message*>* q);
    ///Destructor
    ~Performer();
    ///Queue used to store the received messages
    std::queue<Message*>* message_queue;
    ///Map containing the values
    std::unordered_map<Key, const char*> filesMap;

    /**
     *  This map contains temporary values given by the user, until the FIND
     *  NODE determines the actual targets of the store. When the targets are
     *  found an RPC_STORE request is sent and the value deleted
     */
    std::unordered_map<Key, const std::string> storeTmpMap;

    ///Map containing the class to handle persistence required by the FIND NODE
    std::unordered_map<Key, SearchNode> searchInProgress;

    ///class conatining and handling all the Kbuckets
    NeighbourManager* neighbours;

    ///Return the ThreadID of the thread parsing the messages received
    const pthread_t getThreadID()const;

    ///Print every <Key,Value> pair stored inside this host. ASCII Value only
    void printFilesMap();

    /** \brief Check if the host possess the value
     *
     *  Return true if the current host contains the value for the Key passed as
     *  input
     *
     *  \param[in] key The key for which the value is searched
     */
    bool myselfHasValue(const Key* key);

private:
    pthread_t thread_id;
    pthread_t cleaner_id;
};

///Perform a PING to a Node
void rpc_ping(Node node);
///Perform a PONG to a Node
void rpc_pong(Node node);
///Perform a STORE to the K closest nodes to a Node
void rpc_store_request(const std::string& value, Performer* p);
///Find the K closest nodes to a Node, result will be handled by the performer
void rpc_find_node(const Key* key, Performer* p);
///Find a value, result will be handled by the performer
void rpc_find_value(const Key* key, Performer* p);

///Return the message used to initiate a FIND NODE request
Message generate_find_node_request(const Key* key);
///Return the message used to initiate a FIND NODE request
Message generate_find_node_request(const Node findme);
///Return the message used to answer to a FIND NODE
Message generate_find_node_answer(const Key* key, Kbucket* bucket);

#endif
