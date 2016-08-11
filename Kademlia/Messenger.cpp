#include "Messenger.hpp"
#include "assert.h" //TODO: rimuovere una volta che compila con -DNDEBUG
Ip my_ip("127.0.0.1"); //TODO: trovare il modo di sistemare sta roba
typedef struct listener_thread_params
{
    std::queue<void*>* where_to_write;
    sockaddr_in* listener_process_address;
    socklen_t listener_process_lenght;
    int socket_file_descriptor;
}listener_thread_params;

static void* listener(void* p)
{
    listener_thread_params* params = (listener_thread_params*)p;
    std::queue<void*>* q = params->where_to_write;
    sockaddr_in* listener_address = params->listener_process_address;
    socklen_t list_address_len = params->listener_process_lenght;
    int sockfd = params->socket_file_descriptor;
    free(p); //si, e' brutto deallocare qui, ma tanto sto thread non termina mai
    if(bind(sockfd, (struct sockaddr*)listener_address, list_address_len) < 0)
    {
        fprintf(stderr,"[%s,line %d] %s\n",__FILE__,__LINE__,strerror(errno));
        exit(1);
    }
    char buffer[512];
    ssize_t count;
    count = recv(sockfd,buffer,sizeof(buffer),0);
    if (count==-1)
    {
        fprintf(stderr,"[%s,line %d] %s\n",__FILE__,__LINE__,strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        Message* m = new Message(*(uint32_t*)buffer,*(uint16_t*)(buffer+4),
                                 *(uint16_t*)(buffer+6),(uint8_t*)(buffer+12));
        std::cout<<m->text<<std::endl;
    }
    
    pthread_exit((void*)0);
}

Messenger::Messenger(std::queue<void*>* q, int port_ho)
{
    Messenger::sockfd_recv = socket(AF_INET, SOCK_DGRAM, 0);
    Messenger::sockfd_send = socket(AF_INET, SOCK_DGRAM, 0);
    if(Messenger::sockfd_recv < 0 || Messenger::sockfd_send < 0)
    {
        fprintf(stderr,"[%s,line %d] %s\n",__FILE__,__LINE__,strerror(errno));
        exit(1);
    }
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
}

Messenger::~Messenger()
{ }

void Messenger::sendMessage(const Ip si, int dp, Message& msg)
{
    Messenger::dest.sin_addr.s_addr = si.getIp();
    Messenger::dest.sin_port = htons(dp);
    *(uint32_t*)(msg.text) = (my_ip.getIp());
    //TODO: sostituire Ip() con l'ip effettivo
    *(uint16_t*)(msg.text+4) = htons(Messenger::port_ho);
    *(uint16_t*)(msg.text+6) = msg.length;
    if(sendto(sockfd_send,msg.text,msg.length+12,0,
              (struct sockaddr*)&(Messenger::dest),
              (socklen_t)sizeof(Messenger::dest))==-1)
    {
        fprintf(stderr,"[%s,line %d] %s\n",__FILE__,__LINE__,strerror(errno));
        exit(EXIT_FAILURE);
    }
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

Message::Message(const char* text) : from(my_ip)
{
    memset(Message::text,0,12);//riservo 12 byte per ip e porta
    assert(strlen(text+1)<=500);
    Message::length = strlen(text)+1;
    strncpy(Message::text+12, text, Message::length);
}

Message::Message(const uint8_t* binary_data, short len)
{
    assert(len <= 500);
    memset(Message::text,0,12);//riservo 12 byte per ip e porta
    for(short i=12; i<len+12; i++)
        Message::text[i] = binary_data[i]; //TODO: controllare che non casti
    Message::length = len;
}

Message::Message(const Ip f, uint16_t port_no, short len, uint8_t* data):from(f)
{
    assert(len<=500);
    Message::port_ho = ntohs(port_no);
    Message::length = len;
    strncpy(Message::text,(char*)data,len);
}

const char* Message::getText() const
{
    return (char*)text;
}

Message::~Message()
{ }