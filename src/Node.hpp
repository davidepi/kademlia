#ifndef __NODE_HPP__
#define __NODE_HPP__

#include "Ip.hpp"
#include "Key.hpp"
#include "Spinlock.hpp"
#include <arpa/inet.h>  //htons, htonl,  etc.
#include <sstream>

/** \brief Class used to define a tuple <Ip,port,Key>
 *
 *  This class generates a pair <Ip,port> and then append to it the
 *  corresponding Key.
 */
class Node
{
public:
    
    /** \brief Default constructor
     *
     *  Construct a node with 127.0.0.1 as IP and 0 as port
     */
    Node();
    
    /** \brief Constructor given IP and port
     *
     *  Construct a Node with the given IP and port. The port is expected in
     *  host order.
     *
     *  \param[in] ip The ip of the node
     *  \param[in] port_ho The port of the node in host order
     */
    Node(Ip ip, uint16_t port_ho);
    
    ///Default destructor
    ~Node() = default;
    
    /** \brief Return the IP of this node
     *
     *  \return The IP of this node
     */
    Ip getIp()const;
    
    /** \brief Return the port of this node in host order
     *
     *  \return The port of this node in host order
     */
    uint16_t getPort() const;
    
    /** \brief Return the key of this node
     *
     *  \return The immutable key of this node
     */
    const Key* getKey() const;
    
    /** \brief Check if this node is empty
     *
     *  \return True if the ip is 127.0.0.1 and the port is 0
     */
    bool isEmpty();
    
    /**  \brief Unique comparison between two nodes
     *
     *  \note This comparison is not based upon the Key and does not substitute
     *  in any way the Distance class. It is just an unique comparison in order
     *  to use the Node class in any structure that requires sorting
     */
    bool operator<(const Node&)const;
    
    ///True if this node is equal to the one passed as input
    bool operator==(const Node&)const;
    ///True if this node is different from the one passed as input
    bool operator!=(const Node&)const;

private:
    Ip my_ip;
    uint16_t port_ho;
    Key id;
};

std::ostream& operator<<(std::ostream &strm, const Node& n);

#endif
