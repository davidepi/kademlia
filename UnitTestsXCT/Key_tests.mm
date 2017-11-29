#import <XCTest/XCTest.h>
#include "Key.hpp"
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

- (void)test07_Key_print
{
    //just to have coverage 100%, the print is correct.
    Key a("večer");
    a.print();
}

- (void)test08_Key_craft
{
    Key a("Horká voda se skořicí");
    Key b;
    b.craft(a.getKey());
    XCTAssertTrue(a == b);
}

- (void)test08_Key_ostream
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
    XCTAssertEqual(strcmp("0x000102030405060708090a0b0c0d0e0f10111213",chr),0);
#else
    XCTAssertTrue(false);
#endif
}


@end
