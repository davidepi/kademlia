#include "Performer.hpp"
#include "Node.hpp"

#define TYPE_ERROR {fprintf(stderr,"[%s,line %d]%s\n",__FILE__,__LINE__,\
        strerror(errno));exit(EXIT_FAILURE);}

void rpc_pong(Node node){
	//create message
	Message response("ACK_PING");
	response.setFlags(RPC_PONG);

	//send PONG
	Messenger* m = &(Messenger::getInstance());
	m->sendMessage(node.getIp(), node.getPort(), response);
}

void rpc_ping(Node node){
	//create message
	Message response("PING");
	response.setFlags(RPC_PING);

	//send PING
	Messenger* m = &(Messenger::getInstance());
	m->sendMessage(node.getIp(), node.getPort(), response);
}


static void* execute(void* p) 
{
	std::queue<Message*>* q = (std::queue<Message*>*)p;
	
	std::cout << "Executing... " << std::endl;
	while(1) 
	{
		Message* top;
		if (q->size() > 0)
        {
            top = q->front();
            q->pop();
            Node senderNode(top->getSenderIp(), top->getSenderPort());
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


Performer& Performer::getInstance()
{
    static Performer instance;
    return instance;
}

Performer::Performer()
{ }


void Performer::init(std::queue<Message*>* q)
{
	pthread_create(&(Performer::thread_id), NULL, execute, (void*)q);
}

