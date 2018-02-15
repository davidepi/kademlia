#include "NeighbourManager.hpp"

struct Compare
{
    Compare(const Key* c)
    {
        Compare::c = c;
    }
    const Key* c;

    bool operator()(Node a, Node b)
    {
        return Distance(*a.getKey(), *c) < Distance(*b.getKey(), *c);
    }
};

NeighbourManager::NeighbourManager(Node myself)
{
    NeighbourManager::myself = myself;
}

Node NeighbourManager::getMyself()
{
    return myself;
}

void NeighbourManager::printNeighbours()
{
    for (int i = 0; i < NBYTE * 8; i++)
    {
        if(neighboursArray[i].getSize() > 0)
        {
            std::cout << "KBUCKET NUMBER " << i << std::endl;
        }
        std::list<Node>::const_iterator it;
        for(it = neighboursArray[i].getNodes()->begin();
            it != neighboursArray[i].getNodes()->end(); ++it)
        {
            std::cout << "\tNode " << *it << std::endl;
        }
    }

}

void NeighbourManager::findKClosestNodes(const Key* key, Kbucket* bucket)
{
    findKClosestNodes(key, bucket, false);
}

Node NeighbourManager::findClosestNode(const Key* key)
{
    Kbucket bucket;
    findKClosestNodes(key, &bucket, true);

    std::list<Node>::const_iterator it = bucket.getNodes()->begin();
    return it != bucket.getNodes()->end() ? *it : Node();
}

void NeighbourManager::findKClosestNodes(const Key* key, Kbucket* bucket,
                                         bool justOneNode)
{
    Distance dist(*(myself.getKey()), *key);

    int index = dist.getDistance() - 1;
    if(index < 0)
    {
        index = 0; //if I am new, search for nearest neighbours
    }
    std::list<Node> resultList = *(neighboursArray[index].getNodes());
    assert(resultList.size() <= KBUCKET_SIZE);
    if (resultList.size() == KBUCKET_SIZE || //kbucket full or
        (justOneNode && resultList.size() >= 1)) //one node to find and size > 1
    {
        resultList.sort(Compare(key));
        bucket->setNodes(&resultList);
        return;
    }

    //bucket not full, search in other buckets, farther from me
    int otherBuckIndex = index + 1;
    while(otherBuckIndex < NBYTE * 8 &&
         (resultList.size() < KBUCKET_SIZE ||
         (justOneNode && resultList.size() == 0)))
    {
        addNodesToList(&resultList, otherBuckIndex);
        otherBuckIndex++;
    }

    //bucket still not full, search in other buckets, nearer to me
    otherBuckIndex = index - 1;
    while(otherBuckIndex >= 0 &&
         (resultList.size() < KBUCKET_SIZE ||
         (justOneNode && resultList.size() == 0)))
    {
        addNodesToList(&resultList, otherBuckIndex);
        otherBuckIndex--;
    }
    resultList.sort(Compare(key));
    bucket->setNodes(&resultList);
}

void NeighbourManager::addNodesToList(std::list<Node>* list, int index)
{
    const std::list<Node>* otherList = neighboursArray[index].getNodes();
    std::list<Node>::const_iterator it;
    for(it = otherList->begin(); it != otherList->end(); ++it)
    {
        list->push_back(*it);
        if(list -> size() == KBUCKET_SIZE)
            break;
    }
}

void NeighbourManager::insertNode(const Node node)
{
    Distance dist(myself, node);
    int index = dist.getDistance() - 1;
    if(index < 0)
    {
        return; //not adding myself to the neighbours
    }
    neighboursArray[index].addNode(node);
}
