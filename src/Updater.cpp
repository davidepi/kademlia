#include "Updater.hpp"

struct UpdaterStruct {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::queue<Node> queue;
    std::map<Node, Node> updateNodesMap;
};

struct TimeoutStruct {
    Node oldNode;
    Node newNode;
    std::map<Node, Node>* map;
    Kbucket* kbucket;
    
    TimeoutStruct(Node oldNode, Node newNode, std::map<Node, Node>* map, Kbucket* kbucket) : oldNode(oldNode), newNode(newNode) {
        TimeoutStruct::map = map;
        TimeoutStruct::kbucket = kbucket;
    }
};

void* scan_queue(void* arg);
void* removeAfterTimeout(void* arg);

Updater* Updater::instance = NULL;

Updater* Updater::getInstance () {
    if (instance == NULL) {
        instance = new Updater();
    }
    return instance;
}

Updater::Updater() { 
    vars = new UpdaterStruct();
    pthread_create(&(Updater::thread_id), NULL, scan_queue, (void *)vars);
}


Updater::~Updater() {
    delete vars;
}


void Updater::checkUpdateBucket(Node oldNode, Node newNode, Kbucket* kbucket) {
    //ping least recently seen node
    rpc_ping(oldNode);
    Logger::getInstance().logFormat("ssns", Logger::UPDATER, "Sent ping to ", &oldNode, " to check if it is alive");
    std::cout << "---send ping to port " << oldNode.getPort() << std::endl;
    
    //store the new node to add iff the older node does not answer
    vars->updateNodesMap.insert(std::pair<Node, Node>(oldNode, newNode));

    //launch a thread that waits at least TIMEOUT seconds, 
    //then it substitute the node if a pong has not been received
    pthread_t t_id;
    struct TimeoutStruct* timeoutVars = new TimeoutStruct(oldNode, newNode, &(vars->updateNodesMap), kbucket);
    pthread_create(&t_id, NULL, removeAfterTimeout, (void *)timeoutVars);
}

//add pong to the queue and notify the processing thread
void Updater::processPong(Node n) {
    std::lock_guard<std::mutex> guard(vars->mutex);
    vars->queue.push(n);
    vars->cond_var.notify_all();
}

void* removeAfterTimeout(void* args) {
    sleep(TIMEOUT);
    struct TimeoutStruct* argStruct = (TimeoutStruct*) args;
    
    std::map<Node, Node>::iterator mapIt = argStruct->map->find(argStruct->oldNode);
    
    //if the pair oldNode and newNode matches
    if (mapIt != argStruct->map->end() && (*mapIt).second == argStruct->newNode) {
        std::cout << "+++removing" << std::endl;
        argStruct->map->erase(mapIt);

        //update kbucket
        std::list<Node>* nodeList = argStruct->kbucket->getNodes();

        for (std::list<Node>::const_iterator listIt = nodeList->begin(); listIt != nodeList->end(); ++listIt) {
            if ((*mapIt).first == *listIt) { //delete not answering node, add the new one
                nodeList->remove(*listIt);
                nodeList->push_front((*mapIt).second);
                Logger::getInstance().logFormat("ssn", Logger::UPDATER, "Removing not answering ", &(argStruct->oldNode));
                Logger::getInstance().logFormat("ssns", Logger::UPDATER, "Adding new ", &(argStruct->newNode), " to bucket");
                std::cout << "+++end timeout: the new node was inserted and the old one was removed" << std::endl;
                break;
            }
        }
    }
    
    delete argStruct;
    return NULL;
}

void* scan_queue(void* args) {
    
    struct UpdaterStruct* argStruct = (UpdaterStruct*)args;
        
    while (true) {
        std::unique_lock<std::mutex> mlock(argStruct->mutex);

        while (argStruct->queue.size() == 0) {
            std::cout << "---wait on condition variable" << std::endl;
            argStruct->cond_var.wait(mlock); 
        }
        
        //take first PONG in queue
        Node aliveNode = argStruct->queue.front();
        argStruct->queue.pop();
        char myIp[16];
        aliveNode.getIp().toString(myIp);
        Logger::getInstance().logFormat("ssn", Logger::UPDATER, "Received pong from ", &aliveNode);
        std::cout << "---got pong from " << myIp << " -- " << aliveNode <<  std::endl;

        //find the node who sent the PONG in the map end remove the entry
        std::map<Node, Node>::iterator it;
        it=argStruct->updateNodesMap.find(aliveNode);
        if (it != argStruct->updateNodesMap.end()) {
            argStruct->updateNodesMap.erase(it);
            std::cout << "---removed from tmp map (the node is alive, no need of adding new node)" << std::endl;
        }
        
    }
    return NULL;
}

