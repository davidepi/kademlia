#include "Distance.hpp"
#if !defined(LEFT_DISTANCE) and !defined(RIGHT_DISTANCE)
#define RIGHT_DISTANCE
#endif
Distance::Distance(const Key& k1, const Key& k2)
{
    const uint8_t* key1 = k1.getKey();
    const uint8_t* key2 = k2.getKey();
    
    Distance::value[ 0] = key1[ 0] ^ key2[ 0];
    Distance::value[ 1] = key1[ 1] ^ key2[ 1];
    Distance::value[ 2] = key1[ 2] ^ key2[ 2];
    Distance::value[ 3] = key1[ 3] ^ key2[ 3];
    Distance::value[ 4] = key1[ 4] ^ key2[ 4];
    Distance::value[ 5] = key1[ 5] ^ key2[ 5];
    Distance::value[ 6] = key1[ 6] ^ key2[ 6];
    Distance::value[ 7] = key1[ 7] ^ key2[ 7];
    Distance::value[ 8] = key1[ 8] ^ key2[ 8];
    Distance::value[ 9] = key1[ 9] ^ key2[ 9];
    Distance::value[10] = key1[10] ^ key2[10];
    Distance::value[11] = key1[11] ^ key2[11];
    Distance::value[12] = key1[12] ^ key2[12];
    Distance::value[13] = key1[13] ^ key2[13];
    Distance::value[14] = key1[14] ^ key2[14];
    Distance::value[15] = key1[15] ^ key2[15];
    Distance::value[16] = key1[16] ^ key2[16];
    Distance::value[17] = key1[17] ^ key2[17];
    Distance::value[18] = key1[18] ^ key2[18];
    Distance::value[19] = key1[19] ^ key2[19];
}

Distance::Distance(Node n1, Node n2)
{
    //TODO: non so ancora come sara' la classe node
    //      suppongo ci sara' un metodo getKey() che mi ritorna la chiave
    //      della tupla, e OVVIAMENTE sara' const :)
 
    const uint8_t* key1 = n1.getKey()->getKey();
    const uint8_t* key2 = n2.getKey()->getKey();
    
    Distance::value[0] = key1[0] ^ key2[0];
    Distance::value[1] = key1[1] ^ key2[1];
    Distance::value[2] = key1[2] ^ key2[2];
    Distance::value[3] = key1[3] ^ key2[3];
    Distance::value[4] = key1[4] ^ key2[4];
    Distance::value[5] = key1[5] ^ key2[5];
    Distance::value[6] = key1[6] ^ key2[6];
    Distance::value[7] = key1[7] ^ key2[7];
    Distance::value[8] = key1[8] ^ key2[8];
    Distance::value[9] = key1[9] ^ key2[9];
    Distance::value[10] = key1[10] ^ key2[10];
    Distance::value[11] = key1[11] ^ key2[11];
    Distance::value[12] = key1[12] ^ key2[12];
    Distance::value[13] = key1[13] ^ key2[13];
    Distance::value[14] = key1[14] ^ key2[14];
    Distance::value[15] = key1[15] ^ key2[15];
    Distance::value[16] = key1[16] ^ key2[16];
    Distance::value[17] = key1[17] ^ key2[17];
    Distance::value[18] = key1[18] ^ key2[18];
    Distance::value[19] = key1[19] ^ key2[19];
}

Distance::~Distance()
{ }

bool Distance::operator<(const Distance& k)const
{
    return Distance::getDistance() < k.getDistance();
}

bool Distance::operator>(const Distance& k)const
{
    return k < (*this);
}

bool Distance::operator<=(const Distance& k)const
{
    return !(k > (*this));
}

bool Distance::operator>=(const Distance& k)const
{
    return !(k < (*this));
}

bool Distance::operator==(const Distance& k)const
{
    return Distance::value[ 0] == k.value[ 0] &&
           Distance::value[ 1] == k.value[ 1] &&
           Distance::value[ 2] == k.value[ 2] &&
           Distance::value[ 3] == k.value[ 3] &&
           Distance::value[ 4] == k.value[ 4] &&
           Distance::value[ 5] == k.value[ 5] &&
           Distance::value[ 6] == k.value[ 6] &&
           Distance::value[ 7] == k.value[ 7] &&
           Distance::value[ 8] == k.value[ 8] &&
           Distance::value[ 9] == k.value[ 9] &&
           Distance::value[10] == k.value[10] &&
           Distance::value[11] == k.value[11] &&
           Distance::value[12] == k.value[12] &&
           Distance::value[13] == k.value[13] &&
           Distance::value[14] == k.value[14] &&
           Distance::value[15] == k.value[15] &&
           Distance::value[16] == k.value[16] &&
           Distance::value[17] == k.value[17] &&
           Distance::value[18] == k.value[18] &&
           Distance::value[19] == k.value[19];
}

bool Distance::operator!=(const Distance& k)const
{
    return !((*this) == k);
}

void Distance::printDistance(char* out) const
{
    for(int i=0; i<20;i++)
    {
        out[0+i*8] = ((Distance::value[i]&0x80)>>7)==0?'0':'1';
        out[1+i*8] = ((Distance::value[i]&0x40)>>6)==0?'0':'1';
        out[2+i*8] = ((Distance::value[i]&0x20)>>5)==0?'0':'1';
        out[3+i*8] = ((Distance::value[i]&0x10)>>4)==0?'0':'1';
        out[4+i*8] = ((Distance::value[i]&0x08)>>3)==0?'0':'1';
        out[5+i*8] = ((Distance::value[i]&0x04)>>2)==0?'0':'1';
        out[6+i*8] = ((Distance::value[i]&0x02)>>1)==0?'0':'1';
        out[7+i*8] = (Distance::value[i]&0x01)==0?'0':'1';
    }
    out[160] = '\0';
}

short Distance::getDistance() const
{
#define REVERSE_DISTANCE NBYTE*8
    uint8_t val=0;
#ifdef LEFT_DISTANCE
    uint8_t index=0;
    while(index < NBYTE && val==0)
        val = Distance::value[index++];
    
    if(val == 0) //le chiavi sono uguali
        return 0;
    
    index--;
    
#ifdef BMI1

    uint32_t retval = 0, vall = val;
    
    __asm__ ( "lzcntl %1, %0;"
             :"=r"(retval)
             :"r"(vall << 24)
             :
             );
    
    // lzcnt non esiste per uint8_t, quindi ho castato a uint16_t e shiftato di
    // 8 a sx, senno' il numero di zeri prima del primo uno e' aumentato di 8.
    
    return REVERSE_DISTANCE-((8*index)+(retval)+1);
    
#else //stessa cosa ma scritta non in assembly
      //siccome sono pro lo faccio divide et impera :D
    if((val & 0xF0) > 0) //il primo bit diverso e' tra i 1 e 4
        if((val & 0xC0) > 0) //il primo bit diverso e' tra 1 o 2
            if((val & 0x80) > 0) //la chiave e' 1xxxxxxx
                return REVERSE_DISTANCE-((8*index)+1);
            else    //la chiave e' 01xxxxxx
                return REVERSE_DISTANCE-((8*index)+2);
        else // primo bit diverso e' 3 o 4
            if((val & 0x20) > 0) //la chiave e' 001xxxxx
                return REVERSE_DISTANCE-((8*index)+3);
            else    //la chiave e' 0001xxxx
                return REVERSE_DISTANCE-((8*index)+4);
    else //il primo bit diverso e' tra 5 e 8
        if((val & 0x0C) > 0) //il primo bit diverso e' 5 o 6
            if((val & 0x08) > 0) //la chiave e' 00001xxx
                return REVERSE_DISTANCE-((8*index)+5);
            else    //la chiave e' 000001xx
                return REVERSE_DISTANCE-((8*index)+6);
        else //il primo bit diverso e' 7 o 8
            if((val & 0x02) > 0) //la chiave e' 0000001x
                return REVERSE_DISTANCE-((8*index)+7);
            else 
                return REVERSE_DISTANCE-((8*index)+8); //la chiave e' 00000001
            
    //soluzione per i comuni mortali molto piu' elegante ma non
    //divide et impera :p
    
    //while((val&0xFF)>>(7-i) == 0)i++;
    //return (8*index+i)
#endif
#else
    
    int index=NBYTE-1;
    while(index >=0 && val==0)
        val = Distance::value[index--];
    
    if(val == 0) //le chiavi sono uguali
        return 0;
    
    index++; //due to the previous lines, index is decremented even if the
             //correct value is found
    index = NBYTE-1-index;
    
#ifdef BMI1
    
    uint32_t retval = 0, vall = val;
    
    __asm__ ( "tzcntl %1, %0;"
             :"=r"(retval)
             :"r"(vall)
             :
             );
    return REVERSE_DISTANCE-((8*index)+(retval)+1);
#else
    
    if((val & 0xF) > 0) //il primo bit diverso e' tra i 5 e 8
        if((val & 0x3) > 0) //il primo bit diverso e' tra 7 o 8
            if((val & 0x1) > 0) //la chiave e' xxxxxxx1
                return REVERSE_DISTANCE-(8*index+1);
            else //la chiave e' xxxxxx10
                return REVERSE_DISTANCE-(8*index+2);
        else // il primo bit diverso e' 5 o 6
            if((val & 0x4)>0) //la chiave e' xxxxx100
                return REVERSE_DISTANCE-(8*index+3);
            else //la chaive e' xxxx1000
                return REVERSE_DISTANCE-(8*index+4);
    else //il primo bit diverso e' tra 1 e 4
        if((val & 0x30) > 0) //il primo bit diverso e' 3 o 4
            if((val & 0x10) > 0) //la chiave e' xxx10000
                return REVERSE_DISTANCE-(8*index+5);
            else //la chiave e' xx100000
                return REVERSE_DISTANCE-(8*index+6);
        else //il primo bit diverso e' 1 o 2
            if((val & 0x40) > 0) //la chiave e' x1000000
                return REVERSE_DISTANCE-(8*index+7);
            else // la chiave e' 10000000
                return REVERSE_DISTANCE-(8*index+8);
#endif
#endif
}
