#include <iostream>
#include "Ip.hpp"
#include "Key.hpp"

int main(int argc, const char * argv[])
{
    Key c("255.231.221.203",3142354416);
    const uint32_t* digest = (uint32_t*)c.getKey();
    printf("%#x",digest[0]);
    printf("%x",digest[1]);
    printf("%x",digest[2]);
    printf("%x",digest[3]);
    printf("%x\n",digest[4]);
}
