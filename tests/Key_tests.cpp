#include "../src/Key.hpp"


#include <gtest/gtest.h>
TEST_GROUP(Key)
{ };









TEST(Key,creationWithString)
{

    Key a("key1"); //0xb299ff9ed1c1904a4aa6d0b55c5cdc4d744aee44
    EXPECT_EQ(a.getKey()[0], 0xB2);
    EXPECT_EQ(a.getKey()[1], 0x99);
    EXPECT_EQ(a.getKey()[2], 0xFF);
    EXPECT_EQ(a.getKey()[3], 0x9E);
    EXPECT_EQ(a.getKey()[4], 0xD1);
    EXPECT_EQ(a.getKey()[5], 0xC1);
    EXPECT_EQ(a.getKey()[6], 0x90);
    EXPECT_EQ(a.getKey()[7], 0x4A);
    EXPECT_EQ(a.getKey()[8], 0x4A);
    EXPECT_EQ(a.getKey()[9], 0xA6);
    EXPECT_EQ(a.getKey()[10], 0xD0);
    EXPECT_EQ(a.getKey()[11], 0xB5);
    EXPECT_EQ(a.getKey()[12], 0x5C);
    EXPECT_EQ(a.getKey()[13], 0x5C);
    EXPECT_EQ(a.getKey()[14], 0xDC);
    EXPECT_EQ(a.getKey()[15], 0x4D);
    EXPECT_EQ(a.getKey()[16], 0x74);
    EXPECT_EQ(a.getKey()[17], 0x4A);
    EXPECT_EQ(a.getKey()[18], 0xEE);
    EXPECT_EQ(a.getKey()[19], 0x44);

    Key b("key2"); //0xb0b94cb63c2b200632c90732de6933ebaf1a7d
    EXPECT_EQ(b.getKey()[0], 0xB0);
    EXPECT_EQ(b.getKey()[1], 0xB9);
    EXPECT_EQ(b.getKey()[2], 0x4C);
    EXPECT_EQ(b.getKey()[3], 0xB6);
    EXPECT_EQ(b.getKey()[4], 0x3C);
    EXPECT_EQ(b.getKey()[5], 0x2B);
    EXPECT_EQ(b.getKey()[6], 0x20);
    EXPECT_EQ(b.getKey()[7], 0x06);
    EXPECT_EQ(b.getKey()[8], 0x03);
    EXPECT_EQ(b.getKey()[9], 0x2C);
    EXPECT_EQ(b.getKey()[10], 0x90);
    EXPECT_EQ(b.getKey()[11], 0x73);
    EXPECT_EQ(b.getKey()[12], 0x2D);
    EXPECT_EQ(b.getKey()[13], 0xE6);
    EXPECT_EQ(b.getKey()[14], 0x93);
    EXPECT_EQ(b.getKey()[15], 0x3E);
    EXPECT_EQ(b.getKey()[16], 0xBA);
    EXPECT_EQ(b.getKey()[17], 0xF1);
    EXPECT_EQ(b.getKey()[18], 0xA7);
    EXPECT_EQ(b.getKey()[19], 0x0D);
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

TEST(Key,creationWithText)
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras posuere.");
    Key b("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras pos");

    EXPECT_TRUE(a != b);
}

//- (void) test06
//{
//    srand(time(NULL));
//    static const char alphanum[] =
//    "0123456789"
//    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//    "abcdefghijklmnopqrstuvwxyz";
//    
//    int len = 20;
//    char s[21]; char s2[21];
//    bool found = false;
//    for(int i=0; i<1000;i++)
//    {
//        for (int i = 0; i < len; ++i) {
//            s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
//        }
//        
//        for (int i = 0; i < len; ++i) {
//            s2[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
//        }
//        
//        s[len] = 0;
//        s2[len] = 0;
//        Key a(s);
//        Key b(s2);
//        if((a.key[0] ^ b.key[0]) == 0)
//        {
//            printf("%#x\n",a.key[1] ^ b.key[1]);
//            found = true;
//            break;
//        }
//    }
//    if(found)
//    std::cout<<"Key a(\""<<s<<"\");"<<"Key b(\""<<s2<<"\");"<<std::endl;
//    
//    else
//        std::cout<<"sfigato"<<std::endl;
//}

