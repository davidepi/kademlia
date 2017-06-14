#include "Performer.hpp"
#include "NeighbourManager.hpp"
#include "Updater.hpp"

void rpc_pong(Node node) 
{
    //create message
    Message response(RPC_PONG);

    //send PONG
    (Messenger::getInstance()).sendMessage(node, response);
}

void rpc_ping(Node node) 
{
    //create message
    Message response(RPC_PING);

    //send PING
    (Messenger::getInstance()).sendMessage(node, response);
}

Message generate_find_node_request(const Key* key) {
    Message response(key->getKey(), NBYTE);
    response.setFlags(RPC_FIND_NODE);
    return response;
}

Message generate_find_node_request(Node findme) {
    return generate_find_node_request(findme.getKey());
}

Message generate_find_node_answer(Node findme, Kbucket* bucket)
{
    uint8_t data[500];
    uint32_t ip = findme.getIp().getIp();
    uint16_t port = htons(findme.getPort());
    data[0] = *((uint8_t*)(&ip)+0);
    data[1] = *((uint8_t*)(&ip)+1);
    data[2] = *((uint8_t*)(&ip)+2);
    data[3] = *((uint8_t*)(&ip)+3);
    data[4] = *((uint8_t*)(&port)+0);
    data[5] = *((uint8_t*)(&port)+1);
    int len = bucket->serialize(data+6);
    delete bucket;
    Message response(data,len+6);
    response.setFlags(RPC_FIND_NODE_ANSWER);
    return response;
}

static void* execute(void* this_class) 
{
    Performer* p = (Performer*)this_class;
    std::queue<Message*>* q = p->message_queue;
    std::cout << "Executing... " << std::endl;
    while(1) 
    {
        Message* top;
        if (q->size() > 0)
        {
            top = q->front();
            q->pop();

            Node senderNode = top->getSenderNode();
            
            //update bucket -> add the sender node whichever the RPC is
            p->neighbours->insertNode(&senderNode);
#ifndef NDEBUG
            char ip[16];
            senderNode.getIp().toString(ip);
            printf("Received a message from: %s:%hu\n",ip,senderNode.getPort());
#endif
            switch(top->getFlags())
            {
                case RPC_PING :
                {
                    std::cout << "The message is a ping: " << top->getText() << std::endl;
                    rpc_pong(senderNode);
                    
                }
                    break;
                case RPC_PONG :
                    std::cout << "The message is a pong " << std::endl;
                    Updater::getInstance()->processPong(senderNode);
                    break;
                case RPC_STORE :
                {
                    std::cout << "The message is a store: " << top->getText() << std::endl;

                    std::string key(top->getText(), 20);

                    short textLength = top->getLength();
                    char* text = new char[textLength];
                    for(int i = 20; i < textLength; i++)
                    {
                        text[i] = top->getText()[i];
                    }

                    p->filesMap.insert({{key.c_str(), text}});
                }
                    break;
                case RPC_FIND_NODE :
                {
                    const uint8_t* data = top->getData();
                    uint32_t ip = *((uint32_t*)data);
                    uint16_t port = ntohs(*(uint16_t*)(data+4));
                    Node findme(Ip(ip),port);
#ifndef NDEBUG
                    char stringip[16];
                    findme.getIp().toString(stringip);
                    printf("Searched node: %s:%hu\n",stringip,port);
#endif
                    Key k;
                    //retrieve the key from message
                    k.craft(top->getData()+6);
                    //find closest nodes
                    Kbucket* b = p->neighbours->findKClosestNodes(&k);
                    b->print();
                    Message msg = generate_find_node_answer(findme, b);
                    Messenger::getInstance().sendMessage(senderNode, msg);
                }
                    break;
                    
                case RPC_FIND_NODE_ANSWER :
                {
                    std::cout << "Received a list of nodes" << std::endl;
                    const uint8_t* data = top->getData();
                    uint32_t ip = *((uint32_t*)data);
                    uint16_t port = ntohs(*(uint16_t*)(data+4));
                    Node findme(Ip(ip),port);
#ifndef NDEBUG
                    char stringip[16];
                    findme.getIp().toString(stringip);
                    printf("Searched node: %s:%hu\n",stringip,port);;
#endif
                    Kbucket b(top->getData()+6);
                    SearchNode* sn;
                    std::unordered_map<const Key*,SearchNode*>::const_iterator got = p->searchInProgress.find(findme.getKey());
                    if(got == p->searchInProgress.end())
                    {
                        sn = new SearchNode(findme);
                        p->searchInProgress.insert({{findme.getKey(),sn}});
                    }
                    else
                        sn = got->second;
                    sn->addAnswer(&b);
                    Node askto[ALPHA_REQUESTS];
                    if(sn->queryTo(askto)) //node not found
                    {
                        Message msg = generate_find_node_request(findme);
                        for(int i=0;i<ALPHA_REQUESTS;i++)
                        {
                            Messenger::getInstance().sendMessage(askto[i],msg);
                        }
                    }
                    else //node found
                    {
                        //???
                    }
                }
                    break;
                case RPC_FIND_VALUE :
                {
                    std::cout << "The message is a find value: " << top->getText() << std::endl;
                }
                    break;
                default:
                    //ignore the packet with wrong type flag
                    ;
            }
        } 
        else 
        {
            sleep(2);
        }
    }
    pthread_exit((void*)0);
}

Performer::Performer(std::queue<Message*>* q)
{
    Messenger* m = &(Messenger::getInstance());
    
    Node* myself = new Node(m->getIp(), m->getPort());
    Performer::neighbours = new NeighbourManager(myself);
    
    Performer::message_queue = q;
    pthread_create(&(Performer::thread_id), NULL, execute, (void*)this);
}

const pthread_t Performer::getThreadID()const
{
    return Performer::thread_id;
}

Performer::~Performer()
{ 
    delete neighbours;
}
