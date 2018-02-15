#ifndef __SEARCHNODE_HPP__
#define __SEARCHNODE_HPP__

#include "Node.hpp"
#include "Key.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"
#include "settings.h"
#include "Logger.hpp"
#include "Messenger.hpp"
#include <chrono>
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <assert.h>

//numerical values are used for operator <
enum probeStatus {UNKNOWN = 0, PENDING = 1, ACTIVE = 2};

///Struct used inside the SearchNode to store various information about a node
struct pnode
{
    ///Current Node
    Node node;
    ///Status of the node, if it has yet to be queried, or not
    probeStatus probed;
    ///If the node status is PENDING, this is the time when the request was sent
    std::chrono::system_clock::time_point queryTime;
    ///Distance of the current node to the target Key/Node
    unsigned short distance;
};

/** \brief Class used to ensure persistence between various FIND_NODE calls
 *
 *  In the Kademlia protocol a FIND NODE procedure returns the K closest nodes
 *  known to a given Key/Node. These nodes are the results of several calls. The
 *  caller gives somebody a Key and ask the closest nodes, and after receiving
 *  the answer it repeats the request recursively until saturation.
 *  This class keeps trace of the closest nodes found between various calls.
 */
class SearchNode
{
public:
    /** \brief Construct a SearchNode class given Node and Kbucket
     *
     *  Given the Node to find and a Kbucket containing the initial set of
     *  nodes that will be queried, this class is built
     *
     *  \param[in] n The Node to find
     *  \param[in] add A kbucket containing the initial set of closest nodes
     */
    SearchNode(const Node n, const Kbucket* add);
    
    /** \brief Construct a SearchNode class given a Key and a Kbucket
     *
     *  Given a Key to find and a Kbucket containing the initial set of
     *  nodes that will be queried, this class is built
     *
     *  \param[in] k The Key to find
     *  \param[in] add A kbucket containing the initial set of closest nodes
     */
    SearchNode(const Key* k, const Kbucket* add);
    
    ///Default destructor
    ~SearchNode() = default;
    
    /** \brief Add an answer received from a Node
     *
     *  Mark a node as ACTIVE, since it provided an answer after being asked
     *  for a Kbucket, then add the Kbucket it provided into the list of closest
     *  nodes sorted by distance. The newly added node are marked as UNKNOWN
     *  and if they are close enough they are likely to be queried as well
     *
     *  \param[in] whoanswer The Node that answered with the Kbucket
     *  \param[in] answer The Kbucket provided by the \p whoanswer Node
     */
    void addAnswer(const Node whoanswer, const Kbucket* answer);
    
    /** \brief Remove any expired node
     *
     *  Iterates the various nodes contained and, if too much time has
     *  passed with a Node marked as PENDING, this node is deleted. If the last
     *  PENDING node is deleted, an empty Kbucket is sent to the current machine
     *  otherwise nobody will ever answer again the SearchNode and trigger the
     *  next recursive step
     */
    void clean();
    
    /** \brief Return the list of nodes that shall be queried
     *
     *  Iterates the list of nodes and determine the alpha nodes that shall be
     *  queried next. These nodes will be marked as PENDING. The number of
     *  alpha nodes is returned, if this is 0 the Kbucket is complete. The
     *  maximum number of nodes written is defined in the macro ALPHA_REQUESTS
     *
     *  \param[out] answer The nodes that shall be queries
     *  \return The number of nodes written in the \p answer array
     */
    int queryTo(Node* answer);
    
    /** \brief Return the completed Kbucket
     *
     *  Return the completed Kbucket. Ensure that the method queryTo is
     *  returning 0 before calling this one
     *
     *  \param[out] answer The Kbucket constaining the K closests nodes to the
     *  requested Key.
     */
    void getAnswer(Kbucket* answer);
    
    ///Print this class to stdout
    void print()const;
    ///Return the number of nodes marked as ACTIVE
    int getActive()const;
    ///Return the number of nodes marked as PENDING
    int getPending()const;
    ///Return the number of nodes marked as UNKNOWN
    int getUnknown()const;
    
private:
    Key findkey;
    //nodes list
    std::list<pnode> askme;
    //reserve list
    std::list<pnode> reserve;
    std::mutex mtx;
};

#endif
