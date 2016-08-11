#ifndef __MESSENGER_HPP__
#define __MESSENGER_HPP__

#include <queue>
#include <pthread.h>
#include "Ip.hpp"
#include <string.h>     //memset
#include <sys/types.h> //legacy
#include <sys/socket.h> //socket, AF_INET, SOCK_DGRAM
#include <netinet/in.h> //sockaddr_in, sostituisce arpa/inet.h in alcuni sistemi
#include <arpa/inet.h> //htons, htonl e cosi' via

class Message;
class Messenger
{
    friend class Message;
public:
    Messenger(std::queue<void*>* received_messages, int port_no);
    ~Messenger();
    
    void sendMessage(const Ip destination_ip, int destination_port_ho,
                     Message& msg);
    
private:
    int sockfd_recv;
    int sockfd_send;
    pthread_t thread_id;
    int port_ho; //host order
    sockaddr_in my_address;
    sockaddr_in dest;
};

class Message
{
    friend class Messenger;
public:
    Message(const char* text);
    Message(const uint8_t* binary_data, short len);
    Message(const Ip from, uint16_t port_no, short len, uint8_t* data);
    ~Message();
    const char* getText() const;
    //private:
    char text[512];
    short length;
    Ip from;
    int port_ho; //host order
};

#endif
