#include "Performer.hpp"

#define TYPE_ERROR {fprintf(stderr,"[%s,line %d]%s\n",__FILE__,__LINE__,\
        strerror(errno));exit(EXIT_FAILURE);}

typedef struct execute_thread_params
{
    std::queue<Message*>* where_to_read;
    Messenger* mess_socket;
}execute_thread_params;


static void* execute(void* p) 
{
	execute_thread_params* params = (execute_thread_params*)p;
	std::queue<Message*>* q = params->where_to_read;
	Messenger* m = params->mess_socket;
	std::cout << "Executing... " << std::endl;
	while(1) 
	{
		Message* top;
		if ((top = q->front()) != NULL) {
			q->pop();

			switch(top->getFlags()) {
				case RPC_PING : 
					{
						std::cout << "The message is a ping: " << top->getText() << std::endl;

						Message response("ACK_PING");
						response.setFlags(RPC_PONG);
						m->sendMessage("127.0.0.1", 3400, response);
					}
					break; 
				case RPC_PONG : 
					std::cout << "The message is a pong: " << top->getText() << std::endl;
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
					TYPE_ERROR
			}
		} else {
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


void Performer::init(std::queue<Message*>* q, Messenger* m)
{
	execute_thread_params* params = (execute_thread_params*)malloc
                                     (sizeof(execute_thread_params));
    params->where_to_read = q;
    params->mess_socket = m;
	pthread_create(&(Performer::thread_id), NULL, execute, (void*)params);
}
