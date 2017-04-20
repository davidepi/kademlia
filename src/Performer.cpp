#include "Performer.hpp"
#include "NeighbourManager.hpp"

void rpc_pong(Node node) 
{
    //create message
    Message response(RPC_PONG);

    //send PONG
    Messenger* m = &(Messenger::getInstance());
    m->sendMessage(node, response);
}

void rpc_ping(Node node) 
{
    //create message
    Message response(RPC_PING);

    //send PING
    Messenger* m = &(Messenger::getInstance());
    m->sendMessage(node, response);
}

void Performer::rpc_find_node(Node target_node, Node node, uint8_t iteration)
{
    Message response(iteration);
    response.setFlags(RPC_FIND_NODE);
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
            
            switch(top->getFlags())
            {
                case RPC_PING :
                {
                    std::cout << "The message is a ping: " << top->getText() << std::endl;
                    rpc_pong(senderNode);
                    //find distance and update bucket
                }
                    break;
                case RPC_PONG :
                    std::cout << "The message is a pong: " << top->getText() << std::endl;
                    //update bucket
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
                    std::cout << "The message is a find node: " << top->getText() << std::endl;

                    std::string key(top->getText(), 20);
                    const Key keyToSearch(key.c_str());

                    int i = Distance(*(p->myself->getKey()), keyToSearch).getDistance();
                    std::cout<<"the index is "<<i<<std::endl;
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
    Performer::myself = new Node(m->getIp(), m->getPort());
    
    Performer::neighbours = new NeighbourManager();
    Performer::message_queue = q;
    pthread_create(&(Performer::thread_id), NULL, execute, (void*)this);
}

Performer::~Performer()
{ 
    delete myself;
    delete neighbours;
}
