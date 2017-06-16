#ifndef __NEIGHBOURMANAGER_HPP
#define	__NEIGHBOURMANAGER_HPP

#include "Kbucket.hpp"
#include "Distance.hpp"
#include <assert.h>
#include <algorithm>

class NeighbourManager {
public:
    NeighbourManager(Node* myself);
    ~NeighbourManager();
    void findKClosestNodes(const Key* key, Kbucket* bucket);
    Node findClosestNode(const Key* key);
    const Kbucket* getNeighbourManager();
    const Node* getMyself();
    void insertNode(const Node* node);

private:
    Kbucket neighboursArray[NBYTE * 8];
    Node* myself;
    void addNodesToList(std::list<Node>* list, int index);
    void findKClosestNodes(const Key* key, Kbucket* bucket, bool justOneNode);
};

#endif

