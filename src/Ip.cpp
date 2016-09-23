#include "Ip.hpp"

static inline uint32_t fast_atoi(const char* str)    //~7x faster than atoi
{                                             //mi sembra strano, ma il profiler
    uint32_t val = 0;                         //da questi risultati...
    while(*str)
    {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

Ip::Ip(const char* ip)
{
    if(strcmp(ip,"localhost")==0)
        Ip::ip = 0x100007F;
    else
    {
    char a[4], b[4], c[4], d[4];
    int i = 0,i2 = 0;
    while(ip[i]!='\0' && ip[i]!='.') //trova il primo punto
    {                                //e copia i valori delle 3 cifre in a
        a[i] = ip[i];
        i++;
    }
    a[i++] = '\0';
    while(ip[i]!='\0' && ip[i]!='.') //idem per il secondo terzetto
    {
        b[i2++] = ip[i];
        i++;
    }
    b[i2] = '\0';i++;i2=0;
    while(ip[i]!='\0' && ip[i]!='.') //idem per il terzo
    {
        c[i2++] = ip[i];
        i++;
    }
    c[i2] = '\0';i++;i2=0;
    while(ip[i]!='\0' && ip[i]!='.') //guess what? idem per l'ultimo
    {
        d[i2++] = ip[i];
        i++;
    }
    d[i2] = '\0';
    
    //shifta i numeri. Io voglio che l'ordine dei bit sia [ a ][ b ][ c ][ d ]
    //ma essendo qui little endian devo scrivere 0x[ d ][ c ][ b ][ a ].
    //Oh e' un casino da spiegare sta roba, amen.
    Ip::ip = 0x00000000;
    Ip::ip |= ((unsigned int)fast_atoi(a));
    Ip::ip |= ((unsigned int)fast_atoi(b) << 8 );
    Ip::ip |= ((unsigned int)fast_atoi(c) << 16);
    Ip::ip |= ((unsigned int)fast_atoi(d) << 24);
    }
}

Ip::Ip(int ip_network_ordered)
{
    Ip::ip = ip_network_ordered;
}

Ip::Ip()
{
    Ip::ip = 0x100007F; //assegno 127.0.0.1 come default
}

Ip::~Ip()
{}

uint32_t Ip::getIp() const
{
    return Ip::ip;
}

bool Ip::isLocalhost()const
{
    return Ip::ip == 0x100007F;
}

void Ip::toString(char output[16]) const
{
    uint8_t a = 0x00, b=0x00, c=0x00, d=0x00;
    a = Ip::ip;
    b = Ip::ip >> 8;
    c = Ip::ip >> 16;
    d = Ip::ip >> 24;
    sprintf(output,"%d",a);
    strcat(output,".");
    output = strchr(output,'\0');
    sprintf(output,"%d",b);
    strcat(output,".");
    output = strchr(output,'\0');
    sprintf(output,"%d",c);
    strcat(output,".");
    output = strchr(output,'\0');
    sprintf(output,"%d",d);
}

bool Ip::operator==(const Ip& a)const
{
    return Ip::ip == a.ip;
}

bool Ip::operator!=(const Ip& a)const
{
    return !(*(this)==a);
}