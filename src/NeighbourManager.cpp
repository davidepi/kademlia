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

Kbucket* NeighbourManager::findKClosestNodes(const Key* key) {
    Distance dist(*(myself->getKey()), *key);

    Kbucket* bucket = new Kbucket();

    int index = dist.getDistance() - 1;
    std::list<Node>* resultList = new std::list<Node>();
    *resultList = *(neighboursArray[index].getNodes());

    assert(resultList->size() <= KBUCKET_SIZE);

    if (resultList->size() == KBUCKET_SIZE) {
        bucket->setNodes(resultList);
        return bucket;
    }

    int otherBuckIndex = index + 1;

    while (resultList->size() < KBUCKET_SIZE && otherBuckIndex < NBYTE * 8) {
        addNodesToList(resultList, otherBuckIndex);
        otherBuckIndex++;
    }

    otherBuckIndex = index - 1;
    while (resultList->size() < KBUCKET_SIZE && otherBuckIndex >= 0) {
        addNodesToList(resultList, otherBuckIndex);
        otherBuckIndex--;
    }
    bucket->setNodes(resultList);
    bucket->getNodes()->sort(Compare(key));
    return bucket;

}

Node NeighbourManager::findClosestNode(const Key* key) {
    Distance dist(*(myself->getKey()), *key);

    Kbucket* bucket = new Kbucket();

    int index = dist.getDistance() - 1;
    bucket->setNodes(neighboursArray[index].getNodes());
    bucket->getNodes()->sort(Compare(key));
    
    Node node = *(bucket->getNodes()->begin());
    delete bucket;
    return node;
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
    neighboursArray[dist.getDistance() - 1].add(*node);
}
