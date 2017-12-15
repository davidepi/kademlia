#ifndef __KBUCKET_HPP__
#define __KBUCKET_HPP__

#include <list>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include "Node.hpp"
#include "settings.h"

class Kbucket {
public:
    Kbucket();
    Kbucket(const uint8_t serialized[500]);
    ~Kbucket();
    void addNode(const Node n);
    void deleteNode(const Node n);
    bool replaceNode(const Node oldNode, const Node newNode);
    std::list<Node>* getNodes()const;
    void setNodes(std::list<Node>* nodeList);
    int getSize()const;
    //return the number of bytes written
    int serialize(uint8_t out[500])const;
    bool contains(Node* n)const;
    void print()const;

private:
    std::list<Node>* nodeList;
    std::mutex mtx;
};

#endif
