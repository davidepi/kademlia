#ifndef __KBUCKET_HPP__
#define __KBUCKET_HPP__

#include <list>
#include "Node.hpp"
#include "settings.h"

class Kbucket {
public:
    Kbucket();
    Kbucket(uint8_t serialized[500]);
    ~Kbucket();
    void add(Node n);
    const std::list<Node>* getNodes();
    void setNodes(std::list<Node>* nodeList);
    void serialize(uint8_t out[500])const;
    void print();

private:
    std::list<Node>* nodeList;
};

#endif
