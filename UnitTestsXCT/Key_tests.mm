#import <XCTest/XCTest.h>
#include "../src/Key.hpp"
@interface Key_tests : XCTestCase
#define XCTEST

@end

@implementation Key_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test00_Key_creationWithString
{
    
    Key a("key1"); //0xb299ff9ed1c1904a4aa6d0b55c5cdc4d744aee44
    XCTAssertEqual(a.getKey()[0], 0xB2);
    XCTAssertEqual(a.getKey()[1], 0x99);
    XCTAssertEqual(a.getKey()[2], 0xFF);
    XCTAssertEqual(a.getKey()[3], 0x9E);
    XCTAssertEqual(a.getKey()[4], 0xD1);
    XCTAssertEqual(a.getKey()[5], 0xC1);
    XCTAssertEqual(a.getKey()[6], 0x90);
    XCTAssertEqual(a.getKey()[7], 0x4A);
    XCTAssertEqual(a.getKey()[8], 0x4A);
    XCTAssertEqual(a.getKey()[9], 0xA6);
    XCTAssertEqual(a.getKey()[10], 0xD0);
    XCTAssertEqual(a.getKey()[11], 0xB5);
    XCTAssertEqual(a.getKey()[12], 0x5C);
    XCTAssertEqual(a.getKey()[13], 0x5C);
    XCTAssertEqual(a.getKey()[14], 0xDC);
    XCTAssertEqual(a.getKey()[15], 0x4D);
    XCTAssertEqual(a.getKey()[16], 0x74);
    XCTAssertEqual(a.getKey()[17], 0x4A);
    XCTAssertEqual(a.getKey()[18], 0xEE);
    XCTAssertEqual(a.getKey()[19], 0x44);
    
    Key b("key2"); //0xb0b94cb63c2b200632c90732de6933ebaf1a7d
    XCTAssertEqual(b.getKey()[0], 0xB0);
    XCTAssertEqual(b.getKey()[1], 0xB9);
    XCTAssertEqual(b.getKey()[2], 0x4C);
    XCTAssertEqual(b.getKey()[3], 0xB6);
    XCTAssertEqual(b.getKey()[4], 0x3C);
    XCTAssertEqual(b.getKey()[5], 0x2B);
    XCTAssertEqual(b.getKey()[6], 0x20);
    XCTAssertEqual(b.getKey()[7], 0x06);
    XCTAssertEqual(b.getKey()[8], 0x03);
    XCTAssertEqual(b.getKey()[9], 0x2C);
    XCTAssertEqual(b.getKey()[10], 0x90);
    XCTAssertEqual(b.getKey()[11], 0x73);
    XCTAssertEqual(b.getKey()[12], 0x2D);
    XCTAssertEqual(b.getKey()[13], 0xE6);
    XCTAssertEqual(b.getKey()[14], 0x93);
    XCTAssertEqual(b.getKey()[15], 0x3E);
    XCTAssertEqual(b.getKey()[16], 0xBA);
    XCTAssertEqual(b.getKey()[17], 0xF1);
    XCTAssertEqual(b.getKey()[18], 0xA7);
    XCTAssertEqual(b.getKey()[19], 0x0D);
}

- (void)test01_Key_creationWithIp
{
    Key a("127.0.0.1",3400);
    
}

- (void)test02_Key_stringEqual
{
    Key a("ciao");
    Key b("ciao");
    
    XCTAssertTrue(a == b);
    XCTAssertFalse(a != b);
}

- (void)test03_Key_stringNotEqual
{
    Key a("ciao");
    Key b("miao");
    
    XCTAssertTrue(a != b);
    XCTAssertFalse(a == b);
}

- (void)test03_Key_ipEqual
{
    Key a("127.0.0.1",3400);
    Key b("127.0.0.1",3400);
    
    XCTAssertTrue(a == b);
    XCTAssertFalse(a != b);
}

- (void)test04_Key_ipNotEqual
{
    Key a("127.0.0.1",3200);
    Key b("127.0.0.1",65000);
    
    XCTAssertTrue(a != b);
    XCTAssertFalse(a == b);
}

- (void)test05_Key_ipNotEqual2
{
    Key a("127.0.0.1",3400);
    Key b("10.196.10.10",3400);
    
    XCTAssertTrue(a != b);
    XCTAssertFalse(a == b);
}

- (void)test06_Key_creationWithText
{
    Key a("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras posuere.");
    Key b("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras pos");
    
    XCTAssertTrue(a != b);
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

@end
