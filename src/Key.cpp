#include "Key.hpp"

//used internally, max 500 byte len
static void sha1(const uint8_t* input, uint8_t * output, uint64_t len);

//left rotate, used for sha1 function
static inline uint32_t rotl32(uint32_t value, unsigned char count)
{
#if defined(i386) || defined(__x86_64__)
    __asm__("roll %1, %0":"+g"(value):"cI"(count));
    return value;
#else
    return (value<<count) | (value>>(32-count));
#endif
}

Key::Key(Ip ip, uint16_t portno)
{
    uint8_t input[6]; //assuming 4 byte pid
    uint32_t ipno = ip.getIp();
    input[0] = ipno >> 24;
    input[1] = ipno >> 16;
    input[2] = ipno >> 8;
    input[3] = ipno;
    input[4] = portno >> 8;
    input[5] = portno;
    sha1(input,Key::key,6);
}

Key::Key(const char* name)
{
    sha1((uint8_t*)name,Key::key,strlen(name));
}

const uint8_t* Key::getKey() const
{
    return key;
}

bool Key::operator==(const Key& k) const
{
    return Key::key[0]  == k.key[0]  &&
           Key::key[1]  == k.key[1]  &&
           Key::key[2]  == k.key[2]  &&
           Key::key[3]  == k.key[3]  &&
           Key::key[4]  == k.key[4]  &&
           Key::key[5]  == k.key[5]  &&
           Key::key[6]  == k.key[6]  &&
           Key::key[7]  == k.key[7]  &&
           Key::key[8]  == k.key[8]  &&
           Key::key[9]  == k.key[9]  &&
           Key::key[10] == k.key[10] &&
           Key::key[11] == k.key[11] &&
           Key::key[12] == k.key[12] &&
           Key::key[13] == k.key[13] &&
           Key::key[14] == k.key[14] &&
           Key::key[15] == k.key[15] &&
           Key::key[16] == k.key[16] &&
           Key::key[17] == k.key[17] &&
           Key::key[18] == k.key[18] &&
           Key::key[19] == k.key[19];
}

bool Key::operator!=(const Key& k) const
{
    return !(*this==k);
}

//max 500 byte len. Older commits support full 2^64 range
static void sha1(const uint8_t* input, uint8_t* output, uint64_t len)
{
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;
    uint32_t a; //awful names, but they are the same used in RFC 3174
    uint32_t b; //   ^
    uint32_t c; //   |
    uint32_t d; //   |
    uint32_t e; //   |
    uint32_t f; //   |
    uint32_t k; //   |
    uint32_t temp;//-*
    uint64_t i=0;//iterator for the message
    bool completed = false;
    unsigned char j=0;//iterator for the current 512 bit padded message
    //the additional 1 bit that will be added after the message
    unsigned char additional = 0x80;
    const uint8_t* l = (const uint8_t*) &len;
    uint8_t padded[64];
    uint32_t chunk[80];
    
    //loop
    while(!completed)
    {
        //copy message into padded
        while(i<len && j<64)
            padded[j++] = input[i++];
        //add the additional bit if the message han been completely copied
        //and there is space left
        if(i==len && additional!=0 && j<64)
        {
            padded[j++] = additional;
            additional = 0x0;
        }
        //finished the message
        if(i==len)
        {
            if(j<57) //there is space for the message length
            {
                //length is required in bit, this variable won't be used anymore
                len*=8;
                padded[63] = l[0];
                padded[62] = l[1];
                padded[61] = l[2];
                padded[60] = l[3];
                padded[59] = l[4];
                padded[58] = l[5];
                padded[57] = l[6];
                padded[56] = l[7];
                completed = true;
            }
            else //no space for message length, fill with 0s
                while(j<64)
                    padded[j++] = 0;
        }
        //fill remaining with 0s
        while(j<56)
            padded[j++] = 0;
        //reset for chunk creation
        j=0;
        //break message in 16 words
        for(int k=0;k<16;k++)
        {
            chunk[k]  = 0x0;
            chunk[k] |= padded[j++] << 24;
            chunk[k] |= padded[j++] << 16;
            chunk[k] |= padded[j++] << 8;
            chunk[k] |= padded[j++];
        }
        //extend the words to 80
        for(int k=16;k<80;k++)
            chunk[k] = rotl32(chunk[k-3]^chunk[k-8]^chunk[k-14]^chunk[k-16],1);
        //reset for next iteration
        j = 0;
        //apply the functions
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        k = 0x5A827999;
        for(int l=0;l<20;l++) //function 1
        {
            f = d ^ (b & (c ^ d));
            temp = rotl32(a, 5)+ f + e + k + chunk[l];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0x6ED9EBA1;
        for(int l=20;l<40;l++) //function 2
        {
            f = b ^ c ^ d;
            temp = rotl32(a, 5)+ f + e + k + chunk[l];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0x8F1BBCDC;
        for(int l=40;l<60;l++) //function 3
        {
            f = (b & c) | (d & (b | c));
            temp = rotl32(a, 5)+ f + e + k + chunk[l];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0xCA62C1D6;
        for(int l=60;l<80;l++) //function 4
        {
            f = b ^ c ^ d;
            temp = rotl32(a, 5)+ f + e + k + chunk[l];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }
    
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t* tmp  = (uint8_t*)&h0;
    output[0]  = tmp[3];
    output[1]  = tmp[2];
    output[2]  = tmp[1];
    output[3]  = tmp[0];
    tmp  = (uint8_t*)&h1;
    output[4]  = tmp[3];
    output[5]  = tmp[2];
    output[6]  = tmp[1];
    output[7]  = tmp[0];
    tmp  = (uint8_t*)&h2;
    output[8]  = tmp[3];
    output[9]  = tmp[2];
    output[10] = tmp[1];
    output[11] = tmp[0];
    tmp  = (uint8_t*)&h3;
    output[12] = tmp[3];
    output[13] = tmp[2];
    output[14] = tmp[1];
    output[15] = tmp[0];
    tmp  = (uint8_t*)&h4;
    output[16] = tmp[3];
    output[17] = tmp[2];
    output[18] = tmp[1];
    output[19] = tmp[0];
#else
    uint8_t* tmp  = (uint8_t*)&h0;
    output[0]  = tmp[0];
    output[1]  = tmp[1];
    output[2]  = tmp[2];
    output[3]  = tmp[3];
    tmp  = (uint8_t*)&h1;
    output[4]  = tmp[0];
    output[5]  = tmp[1];
    output[6]  = tmp[2];
    output[7]  = tmp[3];
    tmp  = (uint8_t*)&h2;
    output[8]  = tmp[0];
    output[9]  = tmp[1];
    output[10] = tmp[2];
    output[11] = tmp[3];
    tmp  = (uint8_t*)&h3;
    output[12] = tmp[0];
    output[13] = tmp[1];
    output[14] = tmp[2];
    output[15] = tmp[3];
    tmp  = (uint8_t*)&h4;
    output[16] = tmp[0];
    output[17] = tmp[1];
    output[18] = tmp[2];
    output[19] = tmp[3];
#endif
}

void Key::craft(const uint8_t* bytes)
{
    for(unsigned int i=0;i<NBYTE;i++)
        key[i] = bytes[i];
}

void Key::print()const
{
    printf("%#02x",Key::key[0]);
    printf("%02x",Key::key[1]);
    printf("%02x",Key::key[2]);
    printf("%02x",Key::key[3]);
    printf("%02x",Key::key[4]);
    printf("%02x",Key::key[5]);
    printf("%02x",Key::key[6]);
    printf("%02x",Key::key[7]);
    printf("%02x",Key::key[8]);
    printf("%02x",Key::key[9]);
    printf("%02x",Key::key[10]);
    printf("%02x",Key::key[11]);
    printf("%02x",Key::key[12]);
    printf("%02x",Key::key[13]);
    printf("%02x",Key::key[14]);
    printf("%02x",Key::key[15]);
    printf("%02x",Key::key[16]);
    printf("%02x",Key::key[17]);
    printf("%02x",Key::key[18]);
    printf("%02x\n",Key::key[19]);
}

std::ostream& operator<<(std::ostream& strm, const Key& k)
{
    char key[3+NBYTE*2];
    key[0] = '0';
    key[1] = 'x';
    key[3+NBYTE*2-1] = '\0';
    int j = 0;
    for(int i=2;i<3+NBYTE*2-1;i+=2)
        snprintf(key+i,3+NBYTE*2,"%02x",k.getKey()[j++]);
    return strm << key;
}
