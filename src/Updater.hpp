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
#include "Logger.hpp"

///Struct used inside the Updater
struct UpdaterStruct
{
    ///mutex used for the condition variable
    std::mutex mutex;
    ///condition variable for the PONG queuq
    std::condition_variable cond_var;
    ///queue containing the received PONGs
    std::queue<Node> queue;
    ///map indicating who should be replaced by who, if the first fail to answer
    std::map<Node, Node> updateNodesMap;
};

/** \brief Class used to set a timeout for non answering nodes
 *
 *  When a Kbucket is full, if a new Node communicates with the host, it should
 *  be put on hold, until it is determined if the Kbucket contains a non
 *  answering node that can be removed. This singleton is used to perform this
 *  check asyncronously.
 */
class Updater
{
public:
    ///Return an instance of this singleton
    static Updater& getInstance();
    ///Avoid multiple copies
    Updater(const Updater&) = delete;
    ///Avoid multiple copies
    void operator=(const Updater&) = delete;
    
    /** \brief Check if a node can be removed
     *
     *  This method checks if oldNode is answering, and wait a determined amount
     *  of time asynchronously. If, after this amount of time oldNode still
     *  has not answered, it is removed and replaced with newNode
     *
     *  \param[in] oldNode The node that will be asked for a pong
     *  \param[in] newNode The node that will replace oldNode if it fails to
     *  answer
     *  \param[in] kbucket The bucket where oldNode can be found
     */
    void checkUpdateBucket(Node oldNode, Node newNode, Kbucket* kbucket);

    /** \brief Add a PONG to the queue
     *
     *  After receiving a PONG message, this method should be called, so the
     *  Updater can know who answered. Ideally, every PONG message should be
     *  forwarded to the Updater
     *
     *  \param[in] n The node that answered with a PONG
     */
    void processPong(Node n);

private:
    Updater();
    pthread_t thread_id;
    struct UpdaterStruct vars;
};

#endif
