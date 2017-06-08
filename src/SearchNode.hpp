#ifndef __SEARCHNODE_HPP__
#define __SEARCHNODE_HPP__

#include "Node.hpp"
#include "Key.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <functional> //hash

void hash_combine(std::size_t& seed, std::size_t value)
{
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Key_hasher {
    std::size_t operator()(const Key c) const {
        std::size_t seed = 0;
        for(int i=0;i<NBYTE;i++) {
            hash_combine(seed, std::hash<int>()(c.getKey()[i]));
        }
        return seed;
    }
};

class SearchNode
{
public:
    SearchNode(Node n);
    ~SearchNode();
    void addAnswer(Kbucket* answer);
    
private:
    Node findme;
    std::vector<Node> askme;
    std::unordered_map<Key,Node,Key_hasher> asked;
    std::mutex mtx;
};

#endif
