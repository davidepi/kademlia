#include <iostream>
#include "Ip.hpp"
#include "Key.hpp"
#include "Messenger.hpp"
#include <unistd.h>
#include <arpa/inet.h>
int main(int argc, const char * argv[])
{
//    Key c("ciao");
//    const uint32_t* digest = (uint32_t*)c.getKey();
//    printf("%#x",digest[0]);
//    printf("%x",digest[1]);
//    printf("%x",digest[2]);
//    printf("%x",digest[3]);
//    printf("%x\n",digest[4]);
    
    std::queue<Message*>* a;
    Messenger* m = &(Messenger::getInstance());
    m->init(a, 3400);
    sleep(1);
    Message msg("Singleton and Makefile! Ole'!");
    msg.setFlags(RPC_PING);
    
    Ip dest("127.0.0.1");
    char dest_string[16];
    dest.toString(dest_string);
    int port_ho_dest = 3400;
    
    while(1)
    {
        m->sendMessage("127.0.0.1", 3400, msg);
        std::cout<<"Sended to "<<dest_string<<":"<<port_ho_dest;
        sleep(1);
    }
}
