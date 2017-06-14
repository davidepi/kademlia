#ifndef __SEARCHNODE_HPP__
#define __SEARCHNODE_HPP__

#include "Node.hpp"
#include "Key.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "settings.h"
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <algorithm>

//numerical values are used for operator <
enum probeStatus {UNKNOWN = 0, PENDING = 1, ACTIVE = 2};

struct pnode
{
    Node node;
    probeStatus probed;
};

class SearchNode
{
public:
    SearchNode(const Node n, const Kbucket* add);
    SearchNode(const Key* k, const Kbucket* add);
    ~SearchNode();
    void addAnswer(const Node whoanswer, const Kbucket* answer);
    int queryTo(Node* answer); //return 0 if the search is completed
                               //otherwise the size of the answer array
private:
    Key findkey;
    std::list<pnode> askme;
    std::list<pnode> reserve;
    std::mutex mtx;
};

#endif
