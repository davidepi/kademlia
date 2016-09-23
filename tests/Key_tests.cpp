#include "../src/Key.hpp"


#include <CppUTest/TestHarness.h>
TEST_GROUP(Key)
{ };









TEST(Key,creationWithString)
{

    Key a("key1"); //0xb299ff9ed1c1904a4aa6d0b55c5cdc4d744aee44
    CHECK_EQUAL(a.getKey()[0], 0xB2);
    CHECK_EQUAL(a.getKey()[1], 0x99);
    CHECK_EQUAL(a.getKey()[2], 0xFF);
    CHECK_EQUAL(a.getKey()[3], 0x9E);
    CHECK_EQUAL(a.getKey()[4], 0xD1);
    CHECK_EQUAL(a.getKey()[5], 0xC1);
    CHECK_EQUAL(a.getKey()[6], 0x90);
    CHECK_EQUAL(a.getKey()[7], 0x4A);
    CHECK_EQUAL(a.getKey()[8], 0x4A);
    CHECK_EQUAL(a.getKey()[9], 0xA6);
    CHECK_EQUAL(a.getKey()[10], 0xD0);
    CHECK_EQUAL(a.getKey()[11], 0xB5);
    CHECK_EQUAL(a.getKey()[12], 0x5C);
    CHECK_EQUAL(a.getKey()[13], 0x5C);
    CHECK_EQUAL(a.getKey()[14], 0xDC);
    CHECK_EQUAL(a.getKey()[15], 0x4D);
    CHECK_EQUAL(a.getKey()[16], 0x74);
    CHECK_EQUAL(a.getKey()[17], 0x4A);
    CHECK_EQUAL(a.getKey()[18], 0xEE);
    CHECK_EQUAL(a.getKey()[19], 0x44);

    Key b("key2"); //0xb0b94cb63c2b200632c90732de6933ebaf1a7d
    CHECK_EQUAL(b.getKey()[0], 0xB0);
    CHECK_EQUAL(b.getKey()[1], 0xB9);
    CHECK_EQUAL(b.getKey()[2], 0x4C);
    CHECK_EQUAL(b.getKey()[3], 0xB6);
    CHECK_EQUAL(b.getKey()[4], 0x3C);
    CHECK_EQUAL(b.getKey()[5], 0x2B);
    CHECK_EQUAL(b.getKey()[6], 0x20);
    CHECK_EQUAL(b.getKey()[7], 0x06);
    CHECK_EQUAL(b.getKey()[8], 0x03);
    CHECK_EQUAL(b.getKey()[9], 0x2C);
    CHECK_EQUAL(b.getKey()[10], 0x90);
    CHECK_EQUAL(b.getKey()[11], 0x73);
    CHECK_EQUAL(b.getKey()[12], 0x2D);
    CHECK_EQUAL(b.getKey()[13], 0xE6);
    CHECK_EQUAL(b.getKey()[14], 0x93);
    CHECK_EQUAL(b.getKey()[15], 0x3E);
    CHECK_EQUAL(b.getKey()[16], 0xBA);
    CHECK_EQUAL(b.getKey()[17], 0xF1);
    CHECK_EQUAL(b.getKey()[18], 0xA7);
    CHECK_EQUAL(b.getKey()[19], 0x0D);
}

TEST(Key,creationWithIp)
{
    Key a("127.0.0.1",3400);

}

TEST(Key,stringEqual)
{
    Key a("ciao");
    Key b("ciao");

    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST(Key,stringNotEqual)
{
    Key a("ciao");
    Key b("miao");

    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST(Key,ipEqual)
{
    Key a("127.0.0.1",3400);
    Key b("127.0.0.1",3400);

    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST(Key,ipNotEqual)
{
    Key a("127.0.0.1",3200);
    Key b("127.0.0.1",65000);

    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST(Key,ipNotEqual2)
{
    Key a("127.0.0.1",3400);
    Key b("10.196.10.10",3400);

    CHECK(a != b);
    CHECK_FALSE(a == b);
}

TEST(Key,creationWithText)
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras posuere.");
    Key b("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras pos");

    CHECK(a != b);
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

