#include "Ip.hpp"
#include "Key.hpp"
#include "Messenger.hpp"
#include "Performer.hpp"
#include "Node.hpp"
#include <unistd.h>
#include <string>
#include <sstream>

int kadUI(Performer* p);

int main(int argc, char* argv[])
{
    int c;
    unsigned short port_host = 0, port_dest = 0;
    Ip gateway;
    bool im_gateway = true;
    bool private_net = false;
    while((c = getopt(argc,argv,"hxi:p:P:")) != -1)
    {
        switch(c)
        {
            case 'h':
            {
                fprintf(stdout,"Flags:\n");
                fprintf(stdout,"-i [char*] The ip to connect to on the remote host\n");
                fprintf(stdout,"-p [ uint] The port to connect to on the remote host\n");
                fprintf(stdout,"-P [ uint] The port to use on this host\n");
                fprintf(stdout,"-x [     ] If gateway, use the private network\n");
                fprintf(stdout,"-h [     ] Print this wonderful help :)\n");
                exit(EXIT_SUCCESS);
                break;
            }
            case 'x': private_net = true;break;
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
        srand((unsigned int)time(NULL));
        port_host = ( rand() % (65536+1024)) + 1024;
    }
    else if(port_host <= 1024)
    {
        fprintf(stderr, "%s\n", "Reserved port. Don't worry, you have almost 64512 free ports between 1024 and 65536 :^)");
        exit(EXIT_FAILURE);
    }

    Node gatewaynode(gateway,port_dest);
    std::queue<Message*> a;

    //creating the thread that waits for incoming packets and passes them to the performer one
    Messenger* m = &(Messenger::getInstance());
    m->init(&a, port_host);
    
    if(im_gateway) {
        if(private_net) {
            m->setPrivate();
            std::cout<<"Private network"<<std::endl;
        } 
        else {
            std::cout<<"Public network"<<std::endl;
        }
    } 
    else {
        if(gateway.isLocalhost() || port_dest == 0) {
            fprintf(stderr, "%s\n", "Missing gateway ip or port");
            exit(EXIT_FAILURE);
        }
        if (gateway.isPrivate()) {
            m->setPrivate();
            std::cout<<"Private network"<<std::endl;
        }        
        else {
            std::cout<<"Public network"<<std::endl;
        }
    }
    
    Performer p(&a);
    if(!im_gateway)
    {
        char myIp[16];
        m->getIp().toString(myIp);
        Key myKey(m->getIp(),m->getPort());
        std::cout << "My ip: " << myIp << std::endl;
        Message msg = generate_find_node_request(&myKey);
        msg.setFlags(msg.getFlags()|FIND_START_FLAG);
        (Messenger::getInstance()).sendMessage(gatewaynode, msg);
    }
   
    while (kadUI(&p));
    return 0;
}


int kadUI(Performer* p) {
    int res = 1;
    std::cout << "Choose a command:" << std::endl;
    std::cout << "[1] Store Value" << std::endl;
    std::cout << "[2] Find Value" << std::endl;
    std::cout << "[3] Find Node" << std::endl;
    std::cout << "[4] Ping" << std::endl;
    std::cout << "[5] Print value map" << std::endl;
    std::cout << "[6] Print kbucket" << std::endl;
    std::cout << "[7] Exit" << std::endl;

    int command;
    std::string commandString;
    getline(std::cin, commandString);
    std::stringstream(commandString) >> command;

    switch (command) {
        case 1:
        {
            std::cout << "Insert the value to store:" << std::endl;
            std::string value;
            std::getline(std::cin, value);
            rpc_store_request(value.c_str(), p);
            break;
        }

        case 2:
        {
            std::cout << "Insert the key to find the value:" << std::endl;
            std::string value;
            std::getline(std::cin, value);
            
            //create the key converting the string to bytes
            uint8_t keyBytes[NBYTE];
            for (int i = 0; i < NBYTE; i++) { 
                keyBytes[i] = (uint8_t)(std::stoi(value.substr(i*2 + 2, 2), nullptr, 16));
            } 
            Key myKey;
            myKey.craft(keyBytes);
            myKey.print();
                        
            rpc_find_value(&myKey, p);
            
            break;
        }

        case 3:
        {
            std::cout << "Insert the node to find:" << std::endl;
            std::string value;
            std::getline(std::cin, value);
            std::cout << "value: " << value << std::endl;
            break;
        }

        case 4:
        {
            std::cout << "Insert the ip:" << std::endl;
            std::string hostname;
            std::getline(std::cin, hostname);
            
            std::cout << "Insert the port:" << std::endl;
            std::string port;
            std::getline(std::cin, port);
            rpc_ping(Node(Ip(hostname.c_str()), atoi(port.c_str())));
            break;
        }
        case 5:
        {
            p->printFilesMap();
            break;
        }
        case 6:
        {
            p->neighbours->printNeighbours();
            break;
        }
        case 7:
        {
            res = 0;
            std::cout << "Bye" << std::endl;
            break;
        }
        default:
        {
            std::cout << "No valid input" << std::endl;
            break;
        }
    }
    
    return res;
}
