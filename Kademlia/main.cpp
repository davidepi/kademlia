#include <iostream>
#include "Ip.hpp"
#include "Key.hpp"
#include "Messenger.hpp"
#include <unistd.h>
#include <arpa/inet.h>
int main(int argc, const char * argv[])
{
//    Key c("255.231.221.203",3142354416);
//    const uint32_t* digest = (uint32_t*)c.getKey();
//    printf("%#x",digest[0]);
//    printf("%x",digest[1]);
//    printf("%x",digest[2]);
//    printf("%x",digest[3]);
//    printf("%x\n",digest[4]);
    
    std::queue<void*>* a;
    Messenger m(a,3200);
    sleep(1);
    Message msg("It's Raining Men! Hallelujah! - It's Raining Men! Amen!");
    m.sendMessage("127.0.0.1", 3200, msg);
    while(1){sleep(3000);}
}
