#include "Ip.hpp"
#include "Key.hpp"
#include "Messenger.hpp"
#include "Performer.hpp"
#include "Node.hpp"
#include <unistd.h>

int main(int argc, char* argv[])
{
    
    int c;
    unsigned short port_host = 0, port_dest = 0;
    Ip gateway;
    bool im_gateway = true;
    while((c = getopt(argc,argv,"hi:p:P:")) != -1)
    {
        switch(c)
        {
            case 'h':
            {
                fprintf(stdout,"Flags:\n");
                fprintf(stdout,"-i [char*] The ip to connect to on the remote host\n");
                fprintf(stdout,"-p [ uint] The port to connect to on the remote host\n");
                fprintf(stdout,"-P [ uint] The port to use on this host\n");
                fprintf(stdout,"-h [     ] Print this wonderful help :)\n");
                exit(EXIT_SUCCESS);
                break;
            }
            case 'p': port_dest = atoi(optarg);break;
            case 'i': gateway = Ip(optarg); im_gateway = false;break;
            case 'P': port_host = atoi(optarg);break;
            case '?':
            {
                if(optopt=='i' || optopt=='p' || optopt=='P')
                    fprintf(stderr,"Option -%c requires an argument\n",optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknow option `-%c`.\n",optopt);
                else
                    fprintf(stderr,"Unknow option character `\\x%x'.\n",optopt);
                exit(EXIT_FAILURE);
            }
        }
    }
    
    if(port_host == 0)
    {
        srand(time(NULL));
        port_host = ( rand() % (65536+1024)) + 1024;
    }
    else if(port_host <= 1024)
    {
        fprintf(stderr, "%s\n", "Reserved port. Don't worry, you have almost 64512 free ports between 1024 and 65536 :)");
        exit(EXIT_FAILURE);
    }
    

    std::queue<Message*> a;

    //creating the thread that waits for incoming packets and passes them to the performer one
    Messenger* m = &(Messenger::getInstance());
    m->init(&a, port_host);
    
    if(!im_gateway)
    {
        if(gateway.isLocalhost() || port_dest == 0)
        {
            fprintf(stderr, "%s\n", "Missing gateway ip or port");
            exit(EXIT_FAILURE);
        }
        else
        {
            Message msg("", RPC_FIND_NODE); //TODO: settare la mia key qui
            m->sendMessage(Node(gateway, port_dest), msg);
        }
    }
    
    //creating the thread that performs all the requests
    Performer p(&a);
    Message msg("chiave svakjv idhkjcneikjvn", RPC_STORE); 
    Message msg1("PING", RPC_PING);
    m->sendMessage(Node(gateway, port_dest), msg);
    m->sendMessage(Node(gateway, port_dest), msg1);
    //wait for input(add key-value, retrieve value)
    while(true)
    {
        sleep(8000);
    }
}
