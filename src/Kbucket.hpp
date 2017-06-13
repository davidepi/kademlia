#ifndef __KBUCKET_HPP__
#define __KBUCKET_HPP__

#include <list>
#include <unistd.h>
#include "Node.hpp"
#include "settings.h"
#include <iostream>

class Kbucket {
public:
    Kbucket();
    Kbucket(const uint8_t serialized[500]);
    ~Kbucket();
    void add(const Node n);
    std::list<Node>* getNodes()const;
    void setNodes(std::list<Node>* nodeList);
    int getSize()const;
    //return the number of bytes written
    int serialize(uint8_t out[500])const;
    bool contains(Node* n)const;
    void print()const;

private:
    std::list<Node>* nodeList;
};

#endif
