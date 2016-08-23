#include "Performer.hpp"

void rpc_pong(Node node)
{
	//create message
	Message response("ACK_PING", RPC_PONG);

	//send PONG
	Messenger* m = &(Messenger::getInstance());
	m->sendMessage(node, response);
}

void rpc_ping(Node node)
{
	//create message
	Message response("PING", RPC_PING);

	//send PING
	Messenger* m = &(Messenger::getInstance());
	m->sendMessage(node, response);
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
           
			switch(top->getFlags())
            {
				case RPC_PING : 
					{
						std::cout << "The message is a ping: " << top->getText() << std::endl;
						rpc_pong(top->getSenderNode());
						//find distance and update bucket
					}
					break; 
				case RPC_PONG : 
					std::cout << "The message is a pong: " << top->getText() << std::endl;
					//update bucket
					break;
				case RPC_STORE : 
					std::cout << "The message is a store: " << top->getText() << std::endl;
					break;
				case RPC_FIND_NODE : 
					std::cout << "The message is a find node: " << top->getText() << std::endl;
					break;
				case RPC_FIND_VALUE : 
					std::cout << "The message is a find value: " << top->getText() << std::endl;
					break;
				default:
                    //ignore the packet with wrong type flag
                    ;
			}
		}
        else
        {
			sleep(5);
		}
	}
	pthread_exit((void*)0);
}

Performer::Performer(std::queue<Message*>* q)
{
    Performer::message_queue = q;
    pthread_create(&(Performer::thread_id), NULL, execute, (void*)this);
}

Performer::~Performer()
{ }
