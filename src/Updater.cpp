#include "Updater.hpp"

void* scan_queue(void* arg);

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
}


void Updater::checkUpdateBucket(Node oldNode, Node newNode) {
    //ping least recently seen node
    Node tmp(Ip("192.168.1.2"),oldNode.getPort()); //TODO delete when it is possible to send msg in local
    rpc_ping(tmp);
    
    std::cout << "---send ping to port " << oldNode.getPort() << std::endl;
    
    //store the new node to add iff the older node does not answer
    vars->updateNodesMap.insert(std::pair<Node, Node>(oldNode, newNode));
    
}

//add pong to the queue and notify the processing thread
void Updater::processPong(Node n) {
    std::lock_guard<std::mutex> guard(vars->mutex);
    vars->queue.push(n);
    vars->cond_var.notify_all();
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
        std::cout << "---got pong from " << myIp << " -- " << aliveNode.getPort() <<  std::endl;

        //find the node who sent the PONG in the map end remove the entry
        std::map<Node, Node>::iterator it;
        it=argStruct->updateNodesMap.find(aliveNode);
        if (it != argStruct->updateNodesMap.end()) {
            argStruct->updateNodesMap.erase(it);
            std::cout << "---removed from map" << std::endl;
        }
        
    }
}

