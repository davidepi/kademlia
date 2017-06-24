#include "Performer.hpp"

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

void rpc_store_request(const char* text, Performer* p) {
    //store text temporarily
    Key key(text);
    p->storeTmpMap.insert({{&key, text}});
    
    //find the closest node known to ask for other closest nodes
    Node node = p->neighbours->findClosestNode(&key);
    if(node.isEmpty()) {
        std::cout << "WARNING: no node found in all kbuckets" << std::endl;
        return;
    } 
    
    //send message to find closest node where to store the data
    Message findNodeMsg = generate_find_node_request(&key);
    (Messenger::getInstance()).sendMessage(node, findNodeMsg);
}

Message generate_find_node_request(const Key* key) {
    Message response(key->getKey(), NBYTE);
    response.setFlags(RPC_FIND_NODE_REQUEST);
    return response;
}

Message generate_find_node_request(const Node findme) {
    return generate_find_node_request(findme.getKey());
}

Message generate_find_node_answer(const Key* key, Kbucket* bucket)
{
    uint8_t data[500-NBYTE];
    Message response(key->getKey(), NBYTE);
    int len = bucket->serialize(data+NBYTE);
    
    response.append(data,len+NBYTE);
    response.setFlags(RPC_FIND_NODE_ANSWER);
    return response;
}

static void* execute(void* this_class) 
{
    Performer* p = (Performer*)this_class;
    std::queue<Message*>* q = p->message_queue;
    while (true) {
        Messenger* m = &(Messenger::getInstance());
        std::unique_lock<std::mutex> mlock(m->mutex);

        while (p->message_queue->size() == 0) {
            m->cond_var.wait(mlock);
        }
        Message* top;
        top = q->front();
        q->pop();

        Node senderNode = top->getSenderNode();

        //update bucket -> add the sender node whichever the RPC is
        p->neighbours->insertNode(&senderNode);
#ifndef NDEBUG
        char ip[16];
        senderNode.getIp().toString(ip);
        printf("Received a message from: %s:%hu\n",ip,(unsigned short)senderNode.getPort());
#endif
        switch(top->getFlags() & RPC_MASK)
        {
            case RPC_PING :
            {
                std::cout << "The message is a ping" << std::endl;
                rpc_pong(senderNode);

            }
                break;
            case RPC_PONG :
                std::cout << "The message is a pong" << std::endl;
                Updater::getInstance()->processPong(senderNode);
                break;
            case RPC_STORE :
            {
                std::cout << "The message is a store: " << top->getText() << std::endl;

                Key key(top->getText(), NBYTE);
                short textLength = top->getLength();
                char* text = new char[textLength];
                for(int i = NBYTE; i < textLength; i++)
                {
                    text[i] = top->getText()[i];
                }

                p->filesMap.insert({{&key, text}});
            }
                break;
            case RPC_FIND_NODE_REQUEST :
            {
                Key key(top->getText(), NBYTE);
                //find closest nodes
                Kbucket kbucket;
                p->neighbours->findKClosestNodes(&key, &kbucket);
                kbucket.print();
                Message msg = generate_find_node_answer(&key, &kbucket);
                Messenger::getInstance().sendMessage(senderNode, msg);
            }
                break;

            case RPC_FIND_NODE_ANSWER :
            {
                std::cout << "Received a list of nodes" << std::endl;
//                    uint32_t ip = *((uint32_t*)data);
//                    uint16_t port = ntohs(*(uint16_t*)(data+4));
//                    Node findme(Ip(ip),port);
//#ifndef NDEBUG
//                    char stringip[16];
//                    findme.getIp().toString(stringip);
//                    printf("Searched node: %s:%hu\n",stringip,port);;
//#endif
//                    Kbucket b(top->getData()+6);
//                    SearchNode* sn;
//                    std::unordered_map<const Key*,SearchNode*>::const_iterator got = p->searchInProgress.find(findme.getKey());
//                    if(got == p->searchInProgress.end())
//                    {
//                        sn = new SearchNode(findme);
//                        p->searchInProgress.insert({{findme.getKey(),sn}});
//                    }
//                    else
//                        sn = got->second;
//                    sn->addAnswer(&b);
//                    Node askto[ALPHA_REQUESTS];
//                    if(sn->queryTo(askto)) //node not found
//                    {
//                        Message msg = generate_find_node_request(findme);
//                        for(int i=0;i<ALPHA_REQUESTS;i++)
//                        {
//                            Messenger::getInstance().sendMessage(askto[i],msg);
//                        }
//                    }
//                    else //node found
//                    {
//                        //???
//                    }
            }
                break;
            default:
                //ignore the packet with wrong type flag
                ;
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
