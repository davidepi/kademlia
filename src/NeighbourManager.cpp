#include "NeighbourManager.hpp"

struct Compare {

    Compare(const Key* c) {
        Compare::c = c;
    }
    const Key* c;

    bool operator()(Node a, Node b) {
        return Distance(*a.getKey(), *c) < Distance(*b.getKey(), *c);
    }
};

NeighbourManager::NeighbourManager(Node* myself) {
    NeighbourManager::myself = myself;
}

NeighbourManager::~NeighbourManager() {
    delete myself;
}

const Kbucket* NeighbourManager::getNeighbourManager() {
    return neighboursArray;
}

const Node* NeighbourManager::getMyself() {
    return myself;
}

void NeighbourManager::findKClosestNodes(const Key* key, Kbucket* bucket) {
    findKClosestNodes(key, bucket, false);
}

Node NeighbourManager::findClosestNode(const Key* key) {
    Kbucket bucket;
    findKClosestNodes(key, &bucket, true);
    
    std::list<Node>::const_iterator it = bucket.getNodes()->begin();
    return it != bucket.getNodes()->end() ? *it : Node();
}

void NeighbourManager::findKClosestNodes(const Key* key, Kbucket* bucket, bool justOneNode) {
    Distance dist(*(myself->getKey()), *key);

    int index = dist.getDistance() - 1;
    std::list<Node> resultList = *(neighboursArray[index].getNodes());

    assert(resultList.size() <= KBUCKET_SIZE);

    if (resultList.size() == KBUCKET_SIZE || (justOneNode && resultList.size() >= 1)) {
        bucket->setNodes(&resultList);
        bucket->getNodes()->sort(Compare(key));
        return;
    }

    int otherBuckIndex = index + 1;

    while (otherBuckIndex < NBYTE * 8 && 
            (resultList.size() < KBUCKET_SIZE || (justOneNode && resultList.size() == 0))) {
        addNodesToList(&resultList, otherBuckIndex);
        otherBuckIndex++;
    }

    otherBuckIndex = index - 1;
    while (otherBuckIndex >= 0 &&
            (resultList.size() < KBUCKET_SIZE || (justOneNode && resultList.size() == 0))) {
        addNodesToList(&resultList, otherBuckIndex);
        otherBuckIndex--;
    }
    bucket->setNodes(&resultList);
    bucket->getNodes()->sort(Compare(key));

}

void NeighbourManager::addNodesToList(std::list<Node>* list, int index) {
    const std::list<Node>* otherList = neighboursArray[index].getNodes();
    for (std::list<Node>::const_iterator it = otherList->begin(); it != otherList->end(); ++it) {
        list->push_back(*it);
        if (list -> size() == KBUCKET_SIZE) {
            break;
        }
    }
}

void NeighbourManager::insertNode(const Node* node) {
    Distance dist(*myself, *node);
    int index = dist.getDistance() - 1;
    if(index < 0) {
        return; //not adding myself to the neighbours
    }
    neighboursArray[index].add(*node);
}
