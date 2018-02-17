#include "Distance.hpp"
#if !defined(LEFT_DISTANCE) and !defined(RIGHT_DISTANCE)
#define LEFT_DISTANCE
#endif
//find the ith different bit
static uint8_t calculate_distance(uint32_t byte);
//reverse distance is used because if the leading bit is different, the
//distance is maximum. So i want a distance of NBYTE*8 - leading zeroes
#define REVERSE_DISTANCE NBYTE*8

Distance::Distance(const Key& k1, const Key& k2)
{
    const uint8_t* key1 = k1.getKey();
    const uint8_t* key2 = k2.getKey();
    uint8_t res = 0;
#ifdef LEFT_DISTANCE
    int i=0;
    while(i<NBYTE)
    {
        res = key1[i] ^ key2[i];
        if(res==0)
            i++;
        else
            break;
    }
    if(i!=NBYTE)
    {
        uint8_t index = calculate_distance(res);
        Distance::distance = REVERSE_DISTANCE-((8*i)+index);
    }
    else
        Distance::distance = 0;
#else
    int i=NBYTE-1;
    while(i>-1)
    {
        res = key1[i] ^ key2[i];
        if(res==0)
            i--;
        else
            break;
    }
    if(i!=-1)
    {
        uint8_t index = calculate_distance(res);
        Distance::distance = REVERSE_DISTANCE-((8*(NBYTE-1-i))+index);
    }
    else
        Distance::distance = 0;
#endif
}

Distance::Distance(Node n1, Node n2)
{
    const uint8_t* key1 = n1.getKey()->getKey();
    const uint8_t* key2 = n2.getKey()->getKey();
    uint8_t res = 0;
#ifdef LEFT_DISTANCE
    int i=0;
    while(i<NBYTE)
    {
        res = key1[i] ^ key2[i];
        if(res==0)
            i++;
        else
            break;
    }
    if(i!=NBYTE)
    {
        uint8_t index = calculate_distance(res);
        Distance::distance = REVERSE_DISTANCE-((8*i)+index);
    }
    else
        Distance::distance = 0;
#else
    int i=NBYTE-1;
    while(i>-1)
    {
        res = key1[i] ^ key2[i];
        if(res==0)
            i--;
        else
            break;
    }
    if(i!=-1)
    {
        uint8_t index = calculate_distance(res);
        Distance::distance = REVERSE_DISTANCE-((8*(NBYTE-1-i))+index);
    }
    else
        Distance::distance = 0;
#endif
}

bool Distance::operator<(const Distance& k)const
{
    return Distance::distance < k.distance;
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
    return Distance::distance==k.distance;
}

bool Distance::operator!=(const Distance& k)const
{
    return !((*this) == k);
}

uint8_t Distance::getDistance() const
{
    return Distance::distance;
}

static uint8_t calculate_distance(uint32_t byte)
{
#ifdef LEFT_DISTANCE
#ifdef __BMI__
    //lzcnt does not exists for  uint8_t, so everything is casted to uint16_t
    //and left-shifted by 8, otherwise the number of leading zeros is increased
    //by 8
    uint32_t retval = 0;
    __asm__ ( "lzcntl %1, %0;"
             :"=r"(retval)
             :"r"(byte << 24)
             :
             );
    return retval+1; //+1 because I want it 1-based
#else
    //lzcnt but not in assembly. If BMI is not supported calling LZCNT/TZCNT
    //calls instead BSR/BSF functions. However the latter are completely
    //different from lzcnt/tzcnt and the result is wrong. Hence the reason of
    //this implementation

    if((byte & 0xF0) > 0)//first different bit is between 1 and 4
        if((byte & 0xC0) > 0)//first different bit is 1 or 2
            if((byte & 0x80) > 0)//key is 1xxxxxxx
                return 1;
            else//key is 01xxxxxx
                return 2;
        else//first different bit is 3 or 4
            if((byte & 0x20) > 0)//key is 001xxxxx
                return 3;
            else//key is 0001xxxx
                return 4;
    else//first different bit is between 5 and 8
        if((byte & 0x0C) > 0)//first different bit is 5 or 6
            if((byte & 0x08) > 0)//key is 00001xxx
                return 5;
            else//key is 000001xx
                return 6;
        else//first different bit is 7 or 8
            if((byte & 0x02) > 0) //key is 0000001x
                return 7;
            else//key is 00000001
                return 8;

    //less efficient solution (not divide et impera)
    //but way more readable
    //  - - - v
        //while((byte&0xFF)>>(7-i) == 0)i++;
        //return i;
#endif
#else
#ifdef __BMI__

    uint32_t retval = 0;
    //tzcntl counts the trailing zeros so there is no need to shift like with
    //the lzcntl
    __asm__ ( "tzcntl %1, %0;"
             :"=r"(retval)
             :"r"(byte)
             :
             );
    return retval+1;
#else

    if((byte & 0xF) > 0)//first different bit is between 5 and 8
        if((byte & 0x3) > 0)//first different bit is 7 or 8
            if((byte & 0x1) > 0)//the key is xxxxxxx1
                return 1;
            else//the key is xxxxxx10
                return 2;
        else//first different bit is 5 or 6
            if((byte & 0x4)>0)//the key is xxxxx100
                return 3;
            else//the key is xxxx1000
                return 4;
    else//first different bit is between 1 and 4
        if((byte & 0x30) > 0)//first different bit is 3 or 4
            if((byte & 0x10) > 0)//the key is xxx10000
                return 5;
            else//the key is xx100000
                return 6;
        else//first different bit is 1 or 2
            if((byte & 0x40) > 0)//the key is x1000000
                return 7;
            else//the key is 10000000
                return 8;
#endif
#endif
}
