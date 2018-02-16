#ifndef __MESSENGER_HPP__
#define __MESSENGER_HPP__

#include <queue>
#include <pthread.h>
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include "Ip.hpp"
#include "Node.hpp"
#include <string.h>     //memset
#include <sys/types.h>  //legacy
#include <sys/socket.h> //socket, AF_INET, SOCK_DGRAM
#include <netinet/in.h> //sockaddr_in, replaces arpa/inet.h in some systems
#include <arpa/inet.h>  //htons, htonl e cosi' via
#include <ifaddrs.h>
#include "settings.h"
#ifdef CURL_FOUND
#include <curl/curl.h>
#endif

#define NULL_QUEUE -2
#define ALREADY_INITIALIZED -3

class Message;

/** \brief Class used to send messages
 *
 *  Messenger class is a singleton that is used to send Message across the
 *  network. The Messages are always UDP packets, and only IPv4 is supported
 */
class Messenger
{
    friend class Message;

public:

    ///Retrieve an instance of this singleton
    static Messenger& getInstance();
    ///Avoid multiple copies
    Messenger(Messenger const&)      = delete;
    ///Avoid multiple copies
    void operator=(Messenger const&) = delete;

    /** \brief Initialize the listening thread
     *
     *  \param[in] queue The queue that will store the received messages
     *  \param[in] port_ho The port that will be used in this host, host ordered
     *  \param[in] isPrivate True if the protocol is expected to run in a LAN
     */
    int init(std::queue<Message*>* queue, int port_ho, bool isPrivate);

    /** \brief Send a message across the network
     *
     *  \param[in] node The addressee of the message
     *  \param[in] msg The message that will be sent over the network
     */
    void sendMessage(const Node node, const Message& msg) const;
    ///Return the IP of this node
    Ip getIp() const;
    ///Return the port of this host, in host order
    uint16_t getPort() const;
    ///set a private IP for this node
    int setPrivate();
    ///Mutex to synchronize with Performer
    std::mutex mutex;
    ///Cond var to synchronize with Performer
    std::condition_variable cond_var;

private:
    Messenger();
    bool initialized;
    int sockfd;
    pthread_t thread_id;
    Ip my_ip;
    int port_ho;
    sockaddr_in my_address;
    sockaddr_in dest;
    std::queue<Message*>* binded_queue;
};

/** \brief Class used to represent a Message sent across the network
 *
 *  This class encloses data, length, sender node and sender port inside a
 *  single message that will be sent with the Messenger class. The maximum size
 *  of the data is 500 byte since the Messenger operates only with UDP frames
 *  and thus minimize the risk of losing part of a message and having to drop
 *  also the correctly received parts.
 *
 *  Sender node and sender port will be automatically added by the Messenger
 *  class upon sending the Message
 */
class Message
{
    friend class Messenger;
public:
    /** \brief Construct an ASCII message
     *
     *  Given an ASCII null terminated c-string, construct a message containing
     *  it
     *
     *  \param[in] text The string that will be contained in the message
     */
    Message(const char* text);

    /** \brief Construct a message given some binary data
     *
     *  Given some binary data and the length of the data array, construct
     *  a Message containing this data
     *
     *  \param[in] binary_data The data that will be written inside the message
     *  \param[in] len The length of the binary data
     */
    Message(const uint8_t* binary_data, short len);

    /** \brief Set the flags for this message
     *
     *  Given some flags, this method creates an empty Message and append the
     *  flags to it
     *
     *  \param[in] flags The flags that will be appended to the message
     */
    Message(uint8_t flags);

    /** \brief Construct a message given data and flags
     *
     *  Given some binary data, the length of the data and the flags, contruct
     *  the Message
     *
     *  \param[in] binary_data The data to be inserted into the message
     *  \param[in] len The length of the data to be inserted into the message
     *  \param[in] flags The flags that will be appended to the message
     */
    Message(const uint8_t* binary_data, short len, uint8_t flags);

    ///Constructor that will be used only by the Messenger listener thread
    Message(const Ip from, uint16_t port_no, short len, uint8_t* data,
            uint8_t flags);

    ///Default destructor
    ~Message() = default;

    /** \brief Return the text contained into the message
     *
     *  Return the data contained inside the Message as a ASCII null terminated
     *  c-string. This actually returns a pointer to the data inside the message
     *  but it is still owned by the message itself
     *
     *  \return A pointer, owned by the message, referencing the data contained
     *  inside it
     */
    const char* getText() const;

    /** \brief Return the data contained into the message
     *
     *  Return the data contained inside the message. This actually returns a
     *  pointer to the data inside the message but it is still owned by the
     *  message itself.
     *
     *  \return A pointer, owned by the message, referencing the data contained
     *  inside it
     */
    const uint8_t* getData() const;

    /** \brief Append some data to the message
     *
     *  Given some binary data and the length of the data array, append it to
     *  the end of the message
     *
     *  \param[in] data The data that will be appended to the message
     *  \param[in] len The length of the data that will be appendede to the
     *  message
     */
    void append(const uint8_t* data, short len);

    /** \brief Set the content of this message
     *
     *  Given an ASCII null terminated c-string, replace the content of this
     *  Message with the given string.
     *
     *  \param[in] text The string that will replace the data of the message
     */
    void setText(const char* text);

    /** \brief Set the content of this message
     *
     *  Given a binary array and its length, replace the content of this
     *  Message with the data contained inside the given array
     *
     *  \param[in] binary_data The new data that will replace the one contained
     *  inside the message
     *  \param[in] len The length of the data passed as input
     */
    void setData(const uint8_t* binary_data, short len);

    ///Get the lenght of this message, reserved bytes are not counted
    short getLength() const;
    ///Return the sender of this Message
    Node getSenderNode() const;
    ///Set the flags of this Message
    void setFlags(uint8_t flags);
    ///Get the flags of this Message
    uint8_t getFlags() const;

private:
    uint8_t text[512];
    uint8_t flags;
    short length;
    Node senderNode;

};

#endif
