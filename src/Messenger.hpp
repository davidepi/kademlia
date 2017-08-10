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
#include <netinet/in.h> //sockaddr_in, sostituisce arpa/inet.h in alcuni sistemi
#include <arpa/inet.h>  //htons, htonl e cosi' via
#include <ifaddrs.h>
#include "settings.h"
#ifdef CURL_FOUND
#include <curl/curl.h>
#endif

#define RPC_PING 0x1
#define RPC_PONG 0x2
#define RPC_STORE 0x3
#define RPC_FIND_NODE_REQUEST 0x4
#define RPC_FIND_NODE_ANSWER 0x5
#define RPC_FIND_NODE_RESPONSE 0x6
#define RPC_MASK 0x7 // ping|pong|store... etc.

#define FIND_START_FLAG 0x8
#define FIND_VALUE_FLAG 0x10
#define FIND_VALUE_FOUND 0x20
#define FIND_STORE_REQUEST 0x40

#define NULL_QUEUE -2
#define ALREADY_INITIALIZED -3

class Message;
class Messenger
{
    friend class Message;

public:
    static Messenger& getInstance();
    Messenger(Messenger const&)      = delete;
    void operator=(Messenger const&) = delete;
    int init(std::queue<Message*>* queue, int port_ho);
    void sendMessage(const Node node, Message& msg);
    Ip getIp() const;
    uint16_t getPort() const;
    std::queue<Message*>* getBindedQueue() const;
    int setPrivate();
    std::mutex mutex;
    std::condition_variable cond_var;

private:
    Messenger();
    bool initialized;
    int sockfd;
    pthread_t thread_id;
    Ip my_ip;
    int port_ho; //host order
    sockaddr_in my_address;
    sockaddr_in dest;
    std::queue<Message*>* binded_queue;
};

class Message
{
    friend class Messenger;
public:
    Message(const char* text);
    Message(const uint8_t* binary_data, short len);
    Message(uint8_t flags);
    Message(const uint8_t* binary_data, short len, uint8_t flags);
    Message(const Ip from, uint16_t port_no, short len, uint8_t* data,
            uint8_t flags); //used by messenger when the received message is constructed
    ~Message();
    
    const char* getText() const;
    const uint8_t* getData() const;
    void append(const uint8_t* data, short len);
    Node getSenderNode() const;
    void setFlags(uint8_t flags);
    short getFlags() const;
    void setText(const char* text);
    void setData(const uint8_t* binary_data, short len);
    short getLength() const;
    
private:
    uint8_t text[512];
    uint8_t flags;
    short length;
    Node senderNode;

};

#endif
