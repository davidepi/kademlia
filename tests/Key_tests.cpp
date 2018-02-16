#include "Key.hpp"
#define XCTEST


#include <gtest/gtest.h>










TEST(Key,creationWithString)
{

    Key a("key1"); //0xb299ff9ed1c1904a4aa6d0b55c5cdc4d744aee44
    EXPECT_EQ(a.getKey()[ 0], 0x10);
    EXPECT_EQ(a.getKey()[ 1], 0x73);
    EXPECT_EQ(a.getKey()[ 2], 0xab);
    EXPECT_EQ(a.getKey()[ 3], 0x6c);
    EXPECT_EQ(a.getKey()[ 4], 0xda);
    EXPECT_EQ(a.getKey()[ 5], 0x4b);
    EXPECT_EQ(a.getKey()[ 6], 0x99);
    EXPECT_EQ(a.getKey()[ 7], 0x1c);
    EXPECT_EQ(a.getKey()[ 8], 0xd2);
    EXPECT_EQ(a.getKey()[ 9], 0x9f);
    EXPECT_EQ(a.getKey()[10], 0x9e);
    EXPECT_EQ(a.getKey()[11], 0x83);
    EXPECT_EQ(a.getKey()[12], 0xa3);
    EXPECT_EQ(a.getKey()[13], 0x07);
    EXPECT_EQ(a.getKey()[14], 0xf3);
    EXPECT_EQ(a.getKey()[15], 0x40);
    EXPECT_EQ(a.getKey()[16], 0x04);
    EXPECT_EQ(a.getKey()[17], 0xae);
    EXPECT_EQ(a.getKey()[18], 0x93);
    EXPECT_EQ(a.getKey()[19], 0x27);

    Key b("key2"); //0xb0b94cb63c2b200632c90732de6933ebaf1a7d
    EXPECT_EQ(b.getKey()[ 0], 0x87);
    EXPECT_EQ(b.getKey()[ 1], 0xba);
    EXPECT_EQ(b.getKey()[ 2], 0x78);
    EXPECT_EQ(b.getKey()[ 3], 0xe0);
    EXPECT_EQ(b.getKey()[ 4], 0xf0);
    EXPECT_EQ(b.getKey()[ 5], 0x3a);
    EXPECT_EQ(b.getKey()[ 6], 0xfc);
    EXPECT_EQ(b.getKey()[ 7], 0xef);
    EXPECT_EQ(b.getKey()[ 8], 0x60);
    EXPECT_EQ(b.getKey()[ 9], 0x65);
    EXPECT_EQ(b.getKey()[10], 0x7f);
    EXPECT_EQ(b.getKey()[11], 0x34);
    EXPECT_EQ(b.getKey()[12], 0x2e);
    EXPECT_EQ(b.getKey()[13], 0xc5);
    EXPECT_EQ(b.getKey()[14], 0x56);
    EXPECT_EQ(b.getKey()[15], 0x73);
    EXPECT_EQ(b.getKey()[16], 0x68);
    EXPECT_EQ(b.getKey()[17], 0xfa);
    EXPECT_EQ(b.getKey()[18], 0xdd);
    EXPECT_EQ(b.getKey()[19], 0x8c);
}

TEST(Key,creationWithIp)
{
    Key a("127.0.0.1",3400);

}

TEST(Key,stringEqual)
{
    Key a("ciao");
    Key b("ciao");

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Key,stringNotEqual)
{
    Key a("ciao");
    Key b("miao");

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(Key,ipEqual)
{
    Key a("127.0.0.1",3400);
    Key b("127.0.0.1",3400);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Key,ipNotEqual)
{
    Key a("127.0.0.1",3200);
    Key b("127.0.0.1",65000);

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(Key,ipNotEqual2)
{
    Key a("127.0.0.1",3400);
    Key b("10.196.10.10",3400);

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(Key,moreThan64Bytes)
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec tempor amet.");

    EXPECT_EQ(a.getKey()[ 0], 0xad);
    EXPECT_EQ(a.getKey()[ 1], 0x41);
    EXPECT_EQ(a.getKey()[ 2], 0x85);
    EXPECT_EQ(a.getKey()[ 3], 0x1d);
    EXPECT_EQ(a.getKey()[ 4], 0x7f);
    EXPECT_EQ(a.getKey()[ 5], 0xb2);
    EXPECT_EQ(a.getKey()[ 6], 0xfe);
    EXPECT_EQ(a.getKey()[ 7], 0x56);
    EXPECT_EQ(a.getKey()[ 8], 0xdf);
    EXPECT_EQ(a.getKey()[ 9], 0x9d);
    EXPECT_EQ(a.getKey()[10], 0xac);
    EXPECT_EQ(a.getKey()[11], 0xa2);
    EXPECT_EQ(a.getKey()[12], 0xc0);
    EXPECT_EQ(a.getKey()[13], 0x12);
    EXPECT_EQ(a.getKey()[14], 0x89);
    EXPECT_EQ(a.getKey()[15], 0xa8);
    EXPECT_EQ(a.getKey()[16], 0x24);
    EXPECT_EQ(a.getKey()[17], 0xe9);
    EXPECT_EQ(a.getKey()[18], 0x10);
    EXPECT_EQ(a.getKey()[19], 0x03);
}

TEST(Key,exactly64Bytes)
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit posuere.");

    EXPECT_EQ(a.getKey()[ 0], 0x5b);
    EXPECT_EQ(a.getKey()[ 1], 0x39);
    EXPECT_EQ(a.getKey()[ 2], 0x1a);
    EXPECT_EQ(a.getKey()[ 3], 0x81);
    EXPECT_EQ(a.getKey()[ 4], 0x2c);
    EXPECT_EQ(a.getKey()[ 5], 0xbe);
    EXPECT_EQ(a.getKey()[ 6], 0x12);
    EXPECT_EQ(a.getKey()[ 7], 0x3e);
    EXPECT_EQ(a.getKey()[ 8], 0x85);
    EXPECT_EQ(a.getKey()[ 9], 0x86);
    EXPECT_EQ(a.getKey()[10], 0xe1);
    EXPECT_EQ(a.getKey()[11], 0xdc);
    EXPECT_EQ(a.getKey()[12], 0x29);
    EXPECT_EQ(a.getKey()[13], 0x11);
    EXPECT_EQ(a.getKey()[14], 0x90);
    EXPECT_EQ(a.getKey()[15], 0xb7);
    EXPECT_EQ(a.getKey()[16], 0x93);
    EXPECT_EQ(a.getKey()[17], 0x00);
    EXPECT_EQ(a.getKey()[18], 0xca);
    EXPECT_EQ(a.getKey()[19], 0x32);
}

TEST(Key,exactly63Bytes)
{
    //no space for additional 0x1 in sha1

    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit nullam.");

    EXPECT_EQ(a.getKey()[ 0], 0xf0);
    EXPECT_EQ(a.getKey()[ 1], 0x4a);
    EXPECT_EQ(a.getKey()[ 2], 0x92);
    EXPECT_EQ(a.getKey()[ 3], 0xe6);
    EXPECT_EQ(a.getKey()[ 4], 0x0d);
    EXPECT_EQ(a.getKey()[ 5], 0xf1);
    EXPECT_EQ(a.getKey()[ 6], 0x27);
    EXPECT_EQ(a.getKey()[ 7], 0xa0);
    EXPECT_EQ(a.getKey()[ 8], 0x30);
    EXPECT_EQ(a.getKey()[ 9], 0xe6);
    EXPECT_EQ(a.getKey()[10], 0x11);
    EXPECT_EQ(a.getKey()[11], 0xa1);
    EXPECT_EQ(a.getKey()[12], 0x64);
    EXPECT_EQ(a.getKey()[13], 0x8d);
    EXPECT_EQ(a.getKey()[14], 0x4b);
    EXPECT_EQ(a.getKey()[15], 0xd3);
    EXPECT_EQ(a.getKey()[16], 0xb0);
    EXPECT_EQ(a.getKey()[17], 0x34);
    EXPECT_EQ(a.getKey()[18], 0xa4);
    EXPECT_EQ(a.getKey()[19], 0xee);
}

TEST(Key,lessThan63MoreThan56)
{
    //space for additional 0x1 but not for length
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing volutpat.");

    EXPECT_EQ(a.getKey()[ 0], 0x2d);
    EXPECT_EQ(a.getKey()[ 1], 0x30);
    EXPECT_EQ(a.getKey()[ 2], 0xbf);
    EXPECT_EQ(a.getKey()[ 3], 0x23);
    EXPECT_EQ(a.getKey()[ 4], 0x2f);
    EXPECT_EQ(a.getKey()[ 5], 0x43);
    EXPECT_EQ(a.getKey()[ 6], 0x56);
    EXPECT_EQ(a.getKey()[ 7], 0x3e);
    EXPECT_EQ(a.getKey()[ 8], 0xa0);
    EXPECT_EQ(a.getKey()[ 9], 0x2d);
    EXPECT_EQ(a.getKey()[10], 0x7d);
    EXPECT_EQ(a.getKey()[11], 0x83);
    EXPECT_EQ(a.getKey()[12], 0x15);
    EXPECT_EQ(a.getKey()[13], 0xd0);
    EXPECT_EQ(a.getKey()[14], 0x70);
    EXPECT_EQ(a.getKey()[15], 0x0e);
    EXPECT_EQ(a.getKey()[16], 0x5b);
    EXPECT_EQ(a.getKey()[17], 0xf3);
    EXPECT_EQ(a.getKey()[18], 0xbd);
    EXPECT_EQ(a.getKey()[19], 0x3c);
}

TEST(Key,creationWithText)
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras posuere.");
    Key b("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras pos");

    EXPECT_TRUE(a != b);
}

TEST(Key,print)
{
    //just to have coverage 100%, the print is correct.
    Key a("večer");
    a.print();
}

TEST(Key,craft)
{
    Key a("Horká voda se skořicí");
    Key b;
    b.craft(a.getKey());
    EXPECT_TRUE(a == b);
}

TEST(Key,ostream)
{
#if NBYTE==20
    uint8_t key[NBYTE];
    for(int i=0;i<NBYTE;i++)
        key[i] = i;
    Key k;
    k.craft(key);

    std::ostringstream stream;
    stream << k;
    std::string str =  stream.str();
    const char* chr = str.c_str();
    EXPECT_EQ(strcmp("0x000102030405060708090a0b0c0d0e0f10111213",chr),0);
#else
    EXPECT_TRUE(false);
#endif
}

