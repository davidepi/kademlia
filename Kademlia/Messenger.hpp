#ifndef __MESSENGER_HPP__
#define __MESSENGER_HPP__

#include <queue>
#include <pthread.h>
#include "Ip.hpp"
#include "Node.hpp"
#include <string.h>     //memset
#include <sys/types.h>  //legacy
#include <sys/socket.h> //socket, AF_INET, SOCK_DGRAM
#include <netinet/in.h> //sockaddr_in, sostituisce arpa/inet.h in alcuni sistemi
#include <arpa/inet.h>  //htons, htonl e cosi' via
#include "settings.h"

#define RPC_PING 0x1
#define RPC_PONG 0x2
#define RPC_STORE 0x4
#define RPC_FIND_NODE 0x8
#define RPC_FIND_VALUE 0x10

class Message;
class Messenger
{
    friend class Message;

public:
    static Messenger& getInstance();
    Messenger(Messenger const&)      = delete;
    void operator=(Messenger const&) = delete;
    void init(std::queue<Message*>* queue, int port_ho);
    void sendMessage(const Node node, Message& msg);

private:
    Messenger();
    int sockfd_recv;
    int sockfd_send;
    pthread_t thread_id;
    Ip my_ip;
    int port_ho; //host order
    sockaddr_in my_address;
    sockaddr_in dest;
};

class Message
{
    friend class Messenger;
public:
    Message(const char* text, uint8_t flags);
    Message(const uint8_t* binary_data, short len);
    Message(const Ip from, uint16_t port_no, short len, uint8_t* data,
            uint8_t flags);
    ~Message();
    const char* getText() const;
    Node getSenderNode() const;
    void setFlags(uint8_t flags);
    short getFlags() const;
private:
    char text[512];
    uint8_t flags;
    short length;
    Node senderNode;

};

#endif
