#include "Messenger.hpp"
#include "assert.h" //TODO: rimuovere una volta che compila anche con -DNDEBUG

#define CRITICAL_ERROR {fprintf(stderr,"[%s,line %d]%s\n",__FILE__,__LINE__,\
        strerror(errno));exit(EXIT_FAILURE);}

#define RESERVED_BYTES 12 //[0-3] source IP
                          //[4-5] source port (the listening one)


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
    free(p); //si, e' brutto deallocare qui, ma tanto sto thread non termina mai
    if(bind(sockfd, (struct sockaddr*)listener_address, list_address_len) < 0)
        CRITICAL_ERROR
    char buffer[512];
    ssize_t count;
    while((count = recv(sockfd,buffer,sizeof(buffer),0)))
    {
        if (count==-1) CRITICAL_ERROR
        else
        {
            uint32_t* b1 = (uint32_t*)buffer; //in realta' e' inutile sto qui,
                                              //ma senza mi da il "don't pun and
                                              //alias" error
            Message* m = new Message(*b1,ntohs(*(uint16_t*)(buffer+4)),
                                     (short)count,(uint8_t*)(buffer+12),
                                     *(uint8_t*)(buffer+6));
            //char from[16];
            //m->getSenderIp().toString(from);
            //std::cout<<"\tReceived \""<<m->getText()<<"\" from "<<
            //from<<":"<<m->getSenderPort()<<" type "<<m->getFlags()<<std::endl;


            if(q->size() < QUEUE_LENGTH)
            {
                q->push(m);
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

int Messenger::init(std::queue<Message*>* q, int port_ho)
{
    if(q == NULL)
        return NULL_QUEUE;
    if(initialized)
        return ALREADY_INITIALIZED;
    else
        initialized = true; //abbasso il rischio di avere 80000 thread
                            //se avvengono tutte chiamate contemporanee.
                            //ancora non ho garanzie, ma chissene
    char myipstring[16];
#if defined(__linux__) || defined(__unix__)
    FILE* fp = popen("hostname -I","r");
#elif defined(__APPLE__)
    FILE* fp = popen("ifconfig | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\\.){3}[0-9]*).*/\\2/p'","r");
#else
    fprintf(stderr, "%s\n", "Os not recognized");
    exit(EXIT_FAILURE);
#endif
    
    //TODO: if not connected, myipstring will fail and the program crash
    fscanf(fp,"%s",myipstring);
    my_ip = Ip(myipstring);
    pclose(fp);
    
    Messenger::binded_queue = q;
    Messenger::sockfd_recv = socket(AF_INET, SOCK_DGRAM, 0);
    Messenger::sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
    if(Messenger::sockfd_recv < 0 || Messenger::sockfd_send < 0)
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
    params->socket_file_descriptor = Messenger::sockfd_recv;
    params->listener_process_address = &(Messenger::my_address);
    params->listener_process_lenght = sizeof(Messenger::my_address);
    pthread_create(&(Messenger::thread_id), NULL, listener, (void*)params);
    return true;
    
}

void Messenger::sendMessage(const Node node, Message& msg)
{
    struct sockaddr_in dest = Messenger::dest;
    dest.sin_addr.s_addr = node.getIp().getIp(); //get ip of the node and convert into an unsigned network order int
    dest.sin_port = htons(node.getPort());
    uint32_t* t = (uint32_t*)(msg.text);
    *t = (Messenger::my_ip.getIp());
    *(uint16_t*)(msg.text+4) = htons(Messenger::port_ho);
    *(uint16_t*)(msg.text+6) = msg.flags;
    if(sendto(sockfd_send,msg.text,msg.length+RESERVED_BYTES,0,
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

std::queue<Message*>* Messenger::getBindedQueue() const
{
    return Messenger::binded_queue;
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


Message::Message(const char* text)
{
    memset(Message::text,0,RESERVED_BYTES);//riservo 12 byte per ip e porta
    assert(strlen(text+1)<=512-RESERVED_BYTES);
    Message::length = strlen(text)+1;
    memcpy(Message::text+RESERVED_BYTES, text, Message::length);
}

Message::Message(const uint8_t* binary_data, short len)
{
    assert(len <= 512-RESERVED_BYTES);
    memset(Message::text,0,RESERVED_BYTES);//riservo 12 byte per ip e porta
    memcpy(Message::text+RESERVED_BYTES, binary_data, len);
    Message::length = len;
}

Message::Message(uint8_t flags)
{
    memset(Message::text,0,RESERVED_BYTES);//riservo 12 byte per ip e porta
    Message::length = 0;
    Message::setFlags(flags);
}

//Usato da Messenger quando riceve un messaggio, per settare da chi l'ha ricevuto

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

short Message::getFlags() const
{
    return (short)flags;
}

const char* Message::getText() const
{
    return ((char*)text)+RESERVED_BYTES;
}

void Message::setText(const char *text)
{
    memset(Message::text,0,RESERVED_BYTES);//riservo 12 byte per ip e porta
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
    memset(Message::text,0,RESERVED_BYTES);//riservo 12 byte per ip e porta
    memcpy(Message::text+RESERVED_BYTES, binary_data, len);
    Message::length = len;
}

Node Message::getSenderNode() const
{
    return senderNode;
}

short Message::getLength() const
{
    return length;
}

Message::~Message()
{ }
