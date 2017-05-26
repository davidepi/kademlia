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
    void add(Node n);
    const std::list<Node>* getNodes();
    void setNodes(std::list<Node>* nodeList);
    //return the number of bytes written
    int serialize(uint8_t out[500])const;
    void print();

private:
    std::list<Node>* nodeList;
};

#endif
