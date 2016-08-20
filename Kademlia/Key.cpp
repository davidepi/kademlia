#include "Key.hpp"

//se non c'e' una #define di una funzione di hashing usa SHA1 come default
#ifndef HASHFN
#define HASHFN SHA1
#endif
#if HASHFN == SHA1
#undef NBYTE
#define NBYTE 20
//TODO: altre funzioni di hash prima dell'else
#else //funzione di hash non riconosciuta
#undef HASHFN
#undef NBYTE
#define HASHFN SHA1
#define NBYTE 20
#endif

static void sha1(const uint8_t* input, uint8_t * output);

uint32_t rotl32 (uint32_t value, unsigned int count) //left rotate
{
    const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value<<count) | (value>>( (-count) & mask ));
}

Key::Key()
{
    Key::key = new uint8_t[NBYTE];
}

Key::Key(Ip ip, int port)
{
    Key::key = new uint8_t[NBYTE];
    uint8_t input[7]; //assuming 4 byte pid
    uint16_t ipi = ip.getIp();
    input[0] = ipi >> 24;
    input[1] = ipi >> 16;
    input[2] = ipi >> 8;
    input[3] = ipi;
    input[4] = port >> 8;
    input[5] = port;
    input[6] = '\0';
#if HASHFN == SHA1
    sha1(input,Key::key);
#else
    //TODO: altre funzioni di hash (magari con output piu' corti per testare)
#endif
}

Key::Key(const char* name)
{
    Key::key = new uint8_t[NBYTE];
    sha1((uint8_t*)name,Key::key);
}

Key::~Key()
{
    delete[] Key::key;
}

const uint8_t* Key::getKey() const
{
    return key;
}

bool Key::operator==(const Key& k) const
{
#if HASHFN == SHA1
    return Key::key[0]  && k.key[0]  &&
           Key::key[1]  && k.key[1]  &&
           Key::key[2]  && k.key[2]  &&
           Key::key[3]  && k.key[3]  &&
           Key::key[4]  && k.key[4]  &&
           Key::key[5]  && k.key[5]  &&
           Key::key[6]  && k.key[6]  &&
           Key::key[7]  && k.key[7]  &&
           Key::key[8]  && k.key[8]  &&
           Key::key[9]  && k.key[9]  &&
           Key::key[10] && k.key[10] &&
           Key::key[11] && k.key[11] &&
           Key::key[12] && k.key[12] &&
           Key::key[13] && k.key[13] &&
           Key::key[14] && k.key[14] &&
           Key::key[15] && k.key[15] &&
           Key::key[16] && k.key[16] &&
           Key::key[17] && k.key[17] &&
           Key::key[18] && k.key[18] &&
           Key::key[19] && k.key[19];
#else //versione generica per chiavi da NBYTE
    bool retval = true;
    for(int i=0;i<NBYTE;i++)
        retval &= (Key::key[i] && k.key[i]);
    return retval;
#endif
}

bool Key::operator!=(const Key& k) const
{
    return !(*this==k);
}

static void sha1(const uint8_t* input, uint8_t* output)
{
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    uint64_t len = strlen((char*)input);
    const uint8_t* l = (const uint8_t*) &len;

    uint8_t messaggione[(len+9)%64==0?(len+9)/64*64:(len+9)/64*64+64];

    //PADDING 0s
    memset(messaggione, 0, sizeof(messaggione));

    int i=0;
    while(i<len)
    {
        messaggione[i] = input[i];
        i++;
    }

    //ADD 1 BIT TO END OF MESSAGE
    messaggione[i++] = 0x1;


    //APPEND ORIGINAL MESSAGE LENGTH IN BIG ENDIAN
    //lo voglio in big endian quindi aggiungo byte per byte dal fondo
    messaggione[sizeof(messaggione)-1] = l[0];
    messaggione[sizeof(messaggione)-2] = l[1];
    messaggione[sizeof(messaggione)-3] = l[2];
    messaggione[sizeof(messaggione)-4] = l[3];
    messaggione[sizeof(messaggione)-5] = l[4];
    messaggione[sizeof(messaggione)-6] = l[5];
    messaggione[sizeof(messaggione)-7] = l[6];
    messaggione[sizeof(messaggione)-8] = l[7];
    const unsigned long nchu = sizeof(messaggione)/64;
    uint32_t chunks[nchu][80];
    int index = 0;

    //BREAK MESSAGGE IN 512bit CHUNKS AND CHUNKS IN 16 32bit WORDS
    for(int i=0;i<nchu;i++)
        for(int j=0;j<16;j++)
        {
            chunks[i][j]  = 0x00000000;
            chunks[i][j] |= messaggione[index++] << 24  & 0xFF000000;
            chunks[i][j] |= messaggione[index++] << 16  & 0x00FF0000;
            chunks[i][j] |= messaggione[index++] << 8   & 0x0000FF00;
            chunks[i][j] |= messaggione[index++]        & 0x000000FF;
        }

    //EXTEND THE WORDS TO 80
    for(int i=0;i<nchu;i++)
        for(int j=16;j<80;j++)
        {
            chunks[i][j] = rotl32(chunks[i][j-3]  ^ chunks[i][j-8]  ^
                                  chunks[i][j-14] ^ chunks[i][j-16],1);
        }

    //MAIN LOOP
    uint32_t a,b,c,d,e,f,k,temp;
    for(int i=0;i<nchu;i++)
    {
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        k = 0x5A827999;
        for(int j=0;j<20;j++) //function 1
        {
            f = d ^ (b & (c ^ d));
            temp = rotl32(a, 5)+ f + e + k + chunks[i][j];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0x6ED9EBA1;
        for(int j=20;j<40;j++) //function 2
        {
            f = b ^ c ^ d;
            temp = rotl32(a, 5)+ f + e + k + chunks[i][j];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0x8F1BBCDC;
        for(int j=40;j<60;j++) //function 3
        {
            f = (b & c) | (d & (b | c));
            temp = rotl32(a, 5)+ f + e + k + chunks[i][j];
            e = d;
            d = c;
            c = rotl32(b, 30);
            b = a;
            a = temp;
        }
        k = 0xCA62C1D6;
        for(int j=60;j<80;j++) //function 4
        {
            f = b ^ c ^ d;
            temp = rotl32(a, 5)+ f + e + k + chunks[i][j];
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
}
