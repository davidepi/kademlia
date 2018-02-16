#include "Messenger.hpp"
#include "assert.h"
#include "Logger.hpp"

#define CRITICAL_ERROR {fprintf(stderr,"[%s,line %d]%s\n",__FILE__,__LINE__,\
        strerror(errno));exit(EXIT_FAILURE);}

#define RESERVED_BYTES 12 //[0-3] source IP
                          //[4-5] source port (the listening one)
                          //[6]   flags

size_t WriteCallback(void* contents,size_t size,size_t nmemb,void* userp);

typedef struct listener_thread_params
{
    std::queue<Message*>* where_to_write;
    sockaddr_in* listener_process_address;
    socklen_t listener_process_lenght;
    int socket_file_descriptor;
}listener_thread_params;

static void* listener(void* p)
{
    listener_thread_params* params = (listener_thread_params*)p;
    std::queue<Message*>* q = params->where_to_write;
    sockaddr_in* listener_address = params->listener_process_address;
    socklen_t list_address_len = params->listener_process_lenght;
    int sockfd = params->socket_file_descriptor;
    free(p); //bad practice, but this thread never ends
    if(bind(sockfd, (struct sockaddr*)listener_address, list_address_len) < 0)
        CRITICAL_ERROR
    char buffer[512];
    ssize_t count;
    sockaddr_in sender;
    socklen_t len = sizeof(sender);
    while((count = recvfrom(sockfd,buffer,sizeof(buffer),0,
                            (sockaddr*)&sender,&len)))
    {
        if (count==-1) CRITICAL_ERROR
        else
        {
            uint32_t* b1 = (uint32_t*)buffer; //error without this line...
            Message* m = new Message(*b1,ntohs(*(uint16_t*)(buffer+4)),
                                     (short)count-RESERVED_BYTES,
                                     (uint8_t*)(buffer+12),
                                     *(uint8_t*)(buffer+6));
            if(q->size() < QUEUE_LENGTH)
            {
                q->push(m);
                (Messenger::getInstance()).cond_var.notify_all();
            }
        }
    }
    pthread_exit((void*)0);
}

Messenger& Messenger::getInstance()
{
    static Messenger instance;
    return instance;
}

Messenger::Messenger()
{ }

int Messenger::init(std::queue<Message*>* q, int port_ho, bool isPrivate)
{
    if(q == NULL)
        return NULL_QUEUE;
    if(initialized)
        return ALREADY_INITIALIZED;
    else
        initialized = true; //not thread-safe, however init should be called
                            //only once anyway
    char myipstring[16];
    myipstring[0] = 0;
    if(!isPrivate)
    {
#ifdef CURL_FOUND
        CURL *curl;

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "http://ipecho.net/plain");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void*)myipstring);
            curl_easy_setopt(curl, CURLOPT_USERAGENT,"libcurl-agent/1.0");

            curl_easy_perform(curl);
            my_ip = Ip(myipstring);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
#else
#warning "No CURL found, public IP won't be resolved"
        Messenger::setPrivate();
#endif
    }
    else
    {
        Messenger::setPrivate();
    }

    Messenger::binded_queue = q;
    Messenger::sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(Messenger::sockfd < 0)
        CRITICAL_ERROR
    memset(&(Messenger::my_address), 0, sizeof(Messenger::my_address));
    memset(&(Messenger::dest), 0, sizeof(Messenger::dest));

    Messenger::my_address.sin_family      = AF_INET;
    Messenger::my_address.sin_addr.s_addr = INADDR_ANY;
    Messenger::port_ho = port_ho;
    Messenger::my_address.sin_port = htons(Messenger::port_ho);
    Messenger::dest.sin_family = AF_INET;

    listener_thread_params* params = (listener_thread_params*)malloc
                                     (sizeof(listener_thread_params));
    params->where_to_write = q;
    params->socket_file_descriptor = Messenger::sockfd;
    params->listener_process_address = &(Messenger::my_address);
    params->listener_process_lenght = sizeof(Messenger::my_address);
    pthread_create(&(Messenger::thread_id), NULL, listener, (void*)params);
    return true;
}

void Messenger::sendMessage(const Node node, const Message& msg) const
{
    uint8_t flags = msg.getFlags();
    //TODO: delete this - the logging should be done at the end of the performer
    Node me(my_ip,port_ho);
    if(node!=me)
    {
        Logger::getInstance().logFormat("ssnsf", Logger::OUTGOING,
                                    "Message to", &node, "with flags:", &flags);
    }
    struct sockaddr_in dest = Messenger::dest;
    dest.sin_addr.s_addr = node.getIp().getIp();//get ip of node in network ord.
    dest.sin_port = htons(node.getPort());
    uint32_t* t = (uint32_t*)(msg.text);
    *t = (Messenger::my_ip.getIp());
    *(uint16_t*)(msg.text+4) = htons(Messenger::port_ho);
    *(uint16_t*)(msg.text+6) = msg.flags;
#ifndef NDEBUG
    char ip[16];
    node.getIp().toString(ip);
    printf("Sending message to: %s:%hu\n",ip,(unsigned short)node.getPort());
#endif
    if(sendto(sockfd,msg.text,msg.length+RESERVED_BYTES,0,
              (struct sockaddr*)&dest,
              (socklen_t)sizeof(dest)) == -1)
    {
        CRITICAL_ERROR
    }
}

Ip Messenger::getIp()const
{
    return Messenger::my_ip;
}

uint16_t Messenger::getPort()const
{
    return Messenger::port_ho;
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


Message::Message(const char* text)
{
    memset(Message::text,0,RESERVED_BYTES);//reserve 12 bytes for ip and port
    assert(strlen(text+1)<=512-RESERVED_BYTES);
    Message::length = strlen(text)+1;
    memcpy(Message::text+RESERVED_BYTES, text, Message::length);
}

Message::Message(const uint8_t* binary_data, short len)
{
    assert(len <= 512-RESERVED_BYTES);
    memset(Message::text,0,RESERVED_BYTES);//reserve 12 bytes for ip and port
    memcpy(Message::text+RESERVED_BYTES, binary_data, len);
    Message::length = len;
}

Message::Message(uint8_t flags)
{
    memset(Message::text,0,RESERVED_BYTES);//reserve 12 bytes for ip and port
    Message::length = 0;
    Message::setFlags(flags);
}

//Used by Messenger to construct the received message
Message::Message(const Ip f, uint16_t port_ho, short len, uint8_t* data,
                 uint8_t flags):senderNode(f, port_ho)
{
    assert(len<=512-RESERVED_BYTES);
    Message::length = len;
    Message::flags = flags;
    memcpy(Message::text+RESERVED_BYTES,(char*)data,len);
}

void Message::setFlags(uint8_t flags)
{
    Message::flags = flags;
}

uint8_t Message::getFlags() const
{
    return flags;
}

const char* Message::getText() const
{
    return ((char*)text)+RESERVED_BYTES;
}

void Message::setText(const char *text)
{
    memset(Message::text,0,RESERVED_BYTES);//reserve 12 bytes for ip and port
    Message::length = 0;
    assert(strlen(text+1)<=512-RESERVED_BYTES);
    Message::length = strlen(text)+1;
    memcpy(Message::text+RESERVED_BYTES, text, Message::length);
}

const uint8_t* Message::getData() const
{
    return text+RESERVED_BYTES;
}

void Message::setData(const uint8_t *binary_data, short len)
{
    assert(len <= 512-RESERVED_BYTES);
    memset(Message::text,0,RESERVED_BYTES);//reserve 12 bytes for ip and port
    Message::length = 0;
    memcpy(Message::text+RESERVED_BYTES, binary_data, len);
    Message::length = len;
}

void Message::append(const uint8_t *data, short len)
{
    assert(Message::length+len <= 512-RESERVED_BYTES);
    memcpy(Message::text+RESERVED_BYTES+Message::length, data, len);
    Message::length+=len;
}

Node Message::getSenderNode() const
{
    return senderNode;
}

short Message::getLength() const
{
    return length;
}

//used by curl to store the global ip
size_t WriteCallback(void* contents,size_t size,size_t nmemb,void* userp)
{
    //hardcodded because my buffer is limited
    char* ipstring = (char*)userp;
    char* cont = (char*)contents;
    for(int i=0;i<16;i++)
    {
        if(cont[i]=='\r'||cont[i]=='\n'||i==15)
        {
            ipstring[i]='\0';
            break;
        }
        else
            ipstring[i] = cont[i];
    }
    return 16;
}

int Messenger::setPrivate()
{
    char myipstring[16];
#if defined(__linux__) || defined(__unix__)
    FILE* fp = popen("hostname -I","r");
#elif defined(__APPLE__)
    FILE* fp = popen("ifconfig | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\\.){3}[0-9]*).*/\\2/p'","r");
#else
    fprintf(stderr, "%s\n", "Os not recognized");
    exit(EXIT_FAILURE);
#endif

    fscanf(fp,"%s",myipstring);
    my_ip = Ip(myipstring);
    pclose(fp);
    return true;
}
