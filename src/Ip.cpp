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
        Ip::ip_no = 0x7F000001;
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
    //quindi essendo che ognuno di questi e' un byte devo shiftarli in modo
    //da posizionarli come sopra in un unico int big-endian (network ordered)
    //Oh e' un casino da spiegare sta roba, amen.
    Ip::ip_no = 0x00000000;
    Ip::ip_no |= ((unsigned int)fast_atoi(a) << 24);
    Ip::ip_no |= ((unsigned int)fast_atoi(b) << 16);
    Ip::ip_no |= ((unsigned int)fast_atoi(c) << 8);
    Ip::ip_no |= ((unsigned int)fast_atoi(d) << 0);
    }
}

Ip::Ip(int ip_network_ordered)
{
    Ip::ip_no = ip_network_ordered;
}

Ip::Ip()
{
    Ip::ip_no = 0x7F000001; //assegno 127.0.0.1 come default
}

Ip::~Ip()
{}

uint32_t Ip::getIp() const
{
    return Ip::ip_no;
}

uint32_t Ip::getIpHo() const
{
    return ntohl(Ip::ip_no);
}

bool Ip::isLocalhost()const
{
    return Ip::ip_no == 0x7F000001;
}

void Ip::toString(char output[16]) const
{
    uint8_t a = 0x00, b=0x00, c=0x00, d=0x00;
    a = Ip::ip_no >> 24;
    b = Ip::ip_no >> 16;
    c = Ip::ip_no >> 8;
    d = Ip::ip_no >> 0;
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
    return Ip::ip_no == a.ip_no;
}

bool Ip::operator!=(const Ip& a)const
{
    return !(*(this)==a);
}

bool Ip::isPrivate()const
{
    uint8_t q1 = (Ip::ip_no & 0xFF000000) >> 24;
    if(q1 == 10 || q1 == 127)
        return true;
    else
    {
        uint8_t q2 = (Ip::ip_no & 0x00FF0000) >> 16;
        if((q1 == 172 && q2 > 15 && q2 < 32) || (q1 == 192 && q2 == 168) ||
           (q1 == 169 && q2 == 254))
            return true;
        else
            return false;
    }
}
