#include "Updater.hpp"

struct TimeoutStruct
{
    Node oldNode;
    Node newNode;
    std::map<Node, Node>* map;
    Kbucket* kbucket;

    TimeoutStruct(Node oldNode, Node newNode, std::map<Node, Node>* map,
                  Kbucket* kbucket) : oldNode(oldNode), newNode(newNode)
    {
        TimeoutStruct::map = map;
        TimeoutStruct::kbucket = kbucket;
    }
};

static void* scan_queue(void* arg);
static void* removeAfterTimeout(void* arg);

Updater& Updater::getInstance()
{
    static Updater instance;
    return instance;
}

Updater::Updater()
{
    pthread_create(&(Updater::thread_id), NULL, scan_queue, (void*)&vars);
}

void Updater::checkUpdateBucket(Node oldNode, Node newNode, Kbucket* kbucket)
{
    //ping least recently seen node
    rpc_ping(oldNode);
    Logger::getInstance().logBoth("ssns", LOGGER_UPDATER, "Sent ping to ",
                                    &oldNode, " to check if it is alive");
    //store the new node to add iff the older node does not answer
    vars.updateNodesMap.insert(std::pair<Node, Node>(oldNode, newNode));

    //launch a thread that waits at least TIMEOUT seconds,
    //then it substitute the node if a pong has not been received
    pthread_t t_id;
    struct TimeoutStruct* timeoutVars = new TimeoutStruct(oldNode, newNode,
                                            &(vars.updateNodesMap), kbucket);
    pthread_create(&t_id, NULL, removeAfterTimeout, (void *)timeoutVars);
}

//add pong to the queue and notify the processing thread
void Updater::processPong(Node n)
{
    std::lock_guard<std::mutex> guard(vars.mutex);
    vars.queue.push(n);
    vars.cond_var.notify_all();
}

void* removeAfterTimeout(void* args)
{
    sleep(TIMEOUT);
    struct TimeoutStruct* argStruct = (TimeoutStruct*) args;

    std::map<Node, Node>::iterator mapIt;
    mapIt = argStruct->map->find(argStruct->oldNode);

    //if the pair oldNode and newNode matches
    if (mapIt != argStruct->map->end() && (*mapIt).second == argStruct->newNode)
    {
        Logger::getInstance().logStdout("snsns","End timeout ",&(mapIt->first),
                                " removed and ",&(mapIt->second)," inserted");
        //timeout expired, node replaced
        argStruct->kbucket->replaceNode(mapIt->first, mapIt->second);
        //removing
        argStruct->map->erase(mapIt);
    }

    delete argStruct;
    return NULL;
}

void* scan_queue(void* args)
{
    UpdaterStruct* argStruct = (UpdaterStruct*)args;
    while (true)
    {
        std::unique_lock<std::mutex> mlock(argStruct->mutex);
        while (argStruct->queue.size() == 0)
        {
            //wait on cond var
            argStruct->cond_var.wait(mlock);
        }

        //take first PONG in queue
        Node aliveNode = argStruct->queue.front();
        argStruct->queue.pop();
        char myIp[16];
        aliveNode.getIp().toString(myIp);
        Logger::getInstance().logBoth("ssn", LOGGER_UPDATER,
                                        "Received pong from ", &aliveNode);

        //find the node who sent the PONG in the map end remove the entry
        std::map<Node, Node>::iterator it;
        it=argStruct->updateNodesMap.find(aliveNode);
        if (it != argStruct->updateNodesMap.end())
        {
            argStruct->updateNodesMap.erase(it);
        }
    }
    return NULL;
}
