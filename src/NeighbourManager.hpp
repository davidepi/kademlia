#ifndef __NEIGHBOURMANAGER_HPP__
#define	__NEIGHBOURMANAGER_HPP__

#include "Kbucket.hpp"
#include "Distance.hpp"
#include <assert.h>
#include <algorithm>

/** \brief Class used to store a collection of Kbuckets
 *
 *  In Kademlia every Node has a collection of Kbuckets used to store its
 *  neighbours. The number of Kbuckets is equal to the number of bits in the
 *  Key class (160 in this implementation, since the Key uses a SHA-1 function).
 *  This class is used to store every Kbucket and insert the node into the right
 *  bucket. It is also used to retrieve buckets containing the nearest nodes to
 *  the searched key.
 */
class NeighbourManager
{
public:

    /** \brief Default constructor
     *
     *  \param[in] myself The node corresponding to the current machine
     *  <IP,port>
     */
    NeighbourManager(Node myself);

    ///Default destructor
    ~NeighbourManager() = default;

    /** \brief Given a Key find the K closest nodes
     *
     *  This method expects as input a Key and returns a Kbucket containing
     *  the K closest nodes to that key. Since this node return a full Kbucket,
     *  it is expected to return the whole bucket containing the nodes at the
     *  same distance of the Key. If this bucket is not completely full, nodes
     *  are added to it until it is. These nodes are taken first from the
     *  buckets farther from the current machine Key, then from the buckets
     *  nearer to it.
     *
     *  \param[in] key The Key used to know the distance from myself. This
     *  distance will determine the returned Kbucket
     *  \param[out] bucket The returned Kbucket
     */
    void findKClosestNodes(const Key* key, Kbucket* bucket);

    /** \brief Given a Key find the closest node
     *
     *  This method works in the same way of findKClosestNodes, except that
     *  instead of returning a full Kbucket, it returns the single closest node.
     *  The node that answered most recently is returned. If every Kbucket is
     *  empty the node <localhost,0> is returned
     *
     *  \param[in] key The Key used to know the distance from myself. This
     *  distance will determine the returned node.
     */
    Node findClosestNode(const Key* key);

    /** \brief Insert a node inside the Neighbours array
     *
     *  Find the correct Kbucket and insert the node inside it. The insertion
     *  policy of the Kbucket class is applied
     *
     *  \param[in] node The node that will be inserted
     */
    void insertNode(const Node node);

    ///Print the whole list of Kbuckets
    void printNeighbours();
    ///Return this machine <IP,port,Key>
    Node getMyself();

private:
    Kbucket neighboursArray[NBYTE * 8];
    Node myself;
    void addNodesToList(std::list<Node>* list, int index);
    void findKClosestNodes(const Key* key, Kbucket* bucket, bool justOneNode);
};

#endif
