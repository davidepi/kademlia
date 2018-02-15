#ifndef __KBUCKET_HPP__
#define __KBUCKET_HPP__

#include <list>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include "Node.hpp"
#include "settings.h"

/** \brief Class used to store a collection of nodes
 *
 *  The Kbucket class is a collection of Node elements. In every message
 *  exchange, when nodes are expected as answer, the Kademlia protocol states
 *  that a Kbucket is returned instead. This collection is of fixed size,
 *  usually 20, determine at compile time.
 *
 *  This class provides the underlying structure as well as methods to add,
 *  remove and serialize nodes from it. The underlying structure consist of
 *  a queue of node, with First In First Out policy, where the first elements
 *  are the one that first provided an answer after being queried
 */
class Kbucket
{
public:

    ///Default constructor
    Kbucket() = default;
    ///Default destructor
    ~Kbucket() = default;

    /** \brief Constructor given a serialized array
     *
     *  Given a serialized array as input, this constructor builds the
     *  corresponding Kbucket.
     *
     *  \param[in] serialized A byte array representing the serialized kbucket
     */
    Kbucket(const uint8_t serialized[500]);

    /** \brief Add a Node to the Kbucket
     *
     *  Add a Node to the underlying structure containing every node. If the
     *  Node is already inside the Kbucket, it is pushed to the beginning of the
     *  queue. If the node is not in queue and there is no space, every other
     *  node insisde the kbucket is queried for an answer and the new node is
     *  inserted only is somebody fails to answer. Otherwise if there is space
     *  in the Kbucket, the new node is simply inserted.
     *
     *  \param[in] n The node that will be inserted
     */
    void addNode(const Node n);

    /** \brief Remove a node from the Kbucket
     *
     *  \param[in] n The node that will be removed
     */
    void deleteNode(const Node n);

    /** \brief Replace a node
     *
     *  Given two nodes, this method replace the old one with the new one. If
     *  the old node is not found, nothing is done. The new node is always
     *  inserted in front of the queue. A boolean value is returned to know if
     *  the node was inserted
     *
     *  \param[in] oldNode The node that will be removed
     *  \param[in] newNode The node that will be inserted
     *  \return True if the \p oldNode has been found and replaced
     */
    bool replaceNode(const Node oldNode, const Node newNode);

    /** \brief Return the list of nodes
     *
     *  Return an immutable std::list containing the Nodes inside this Kbucket.
     *  This list is a reference for the nodes inside this Kbucket and thus
     *  the pointer remains owned by the Kbucket itself
     *
     *  \return The list of nodes inside this Kbucket
     */
    const std::list<Node>* getNodes()const;

    /** \brief Set the list of nodes
     *
     *  Set the nodes contained in this Kbucket as a copy of the one passed as
     *  input.
     *
     *  \param[in] nodes The list of nodes that will be copied inside this
     *  Kbucket
     */
    void setNodes(const std::list<Node>* nodes);

    /** \brief Return the nodes of this Kbucket
     *
     *  Return the list of nodes contained in this Kbucket, this list is
     *  just the pointer to the one used internally by the Kbucket and thus
     *  it cannot be modified.
     *
     *  \return An std::list containing the nodes of this Kbucket
     */
    int getSize()const;

    /** \brief Serialize this Kbucket
     *
     *  Write this Kbucket in a byte array. In order to deserialize it, one
     *  have to call the specific constructor of the Kbucket.
     *
     *  \param[out] out The array where the serialized Kbucket will be written
     *  \return The number of bytes written
     */
    int serialize(uint8_t out[500])const;

    /** \brief Check if this Kbucket contains a specific node
     *
     *  \param[in] n The node that will be checked
     *  \return True if the input node is contained in the Kbucket
     */
    bool contains(Node* n)const;

    ///Print this Kbucket to stdout
    void print()const;

private:
    std::list<Node> nodeList;
    std::mutex mtx;
};

#endif
