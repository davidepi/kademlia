#ifndef __SEARCHNODE_HPP__
#define __SEARCHNODE_HPP__

#include "Node.hpp"
#include "Key.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "settings.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <algorithm>

class SearchNode
{
public:
    SearchNode(Node n);
    SearchNode(Key* k);
    ~SearchNode();
    void addAnswer(Kbucket* answer);
    int queryTo(Node* answer); //return 0 if the search is completed
                               //otherwise the size of the answer array
    
private:
    Key findkey;
    std::vector<Node> askme;
    std::unordered_map<const Key*,Node> asked;
    std::mutex mtx;
    bool search_ended;
};

#endif
