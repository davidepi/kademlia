#import <XCTest/XCTest.h>
#include "../src/Ip.hpp"

@interface Ip_tests : XCTestCase

@end

@implementation Ip_tests

- (void)setUp {[super setUp];}

- (void)tearDown {[super tearDown];
}

- (void)test00_Ip_creationEmpty
{
    Ip a;
    XCTAssertEqual(a.getIp(), 0x100007F);
}

- (void)test01_Ip_creationGivenNetworkOrdered
{
    int i1 = 0x64839504;
    int i2 = 0x84759354;
    int i3 = 0x74868658;
    int i4 = 0x01930458;
    Ip a(i1);
    Ip b(i2);
    Ip c(i3);
    Ip d(i4);
    XCTAssertEqual(a.getIp(), i1);
    XCTAssertEqual(b.getIp(), i2);
    XCTAssertEqual(c.getIp(), i3);
    XCTAssertEqual(d.getIp(), i4);
}

- (void)test02_Ip_Localhost
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");
    
    XCTAssertEqual(a.getIp(), 0x100007F);
    XCTAssertEqual(b.getIp(), 0x100007F);
    XCTAssertEqual(c.getIp(), 0x100007F);
}

-(void)test03_Ip_LocalhostFunctionChecking
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");
    Ip d("10.194.245.3");
    
    XCTAssertTrue(a.isLocalhost());
    XCTAssertTrue(b.isLocalhost());
    XCTAssertTrue(c.isLocalhost());
    XCTAssertFalse(d.isLocalhost());
}

- (void)test04_Ip_assignOperator
{
    Ip a;
    Ip b("10.194.30.240");
    
    a = b;
    XCTAssertEqual(a.getIp(), b.getIp());
}

- (void)test05_Ip_equalOperator
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");
    
    XCTAssertTrue(a == b);
    XCTAssertFalse(a == c);
}

- (void)test06_Ip_notEqualOperator
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");
    
    XCTAssertTrue(a != c);
    XCTAssertFalse(a != b);
}

- (void)test07_Ip_toStringOperator
{
    Ip a;
    char s[] = "10.194.255.255";
    Ip b(s);
    
    char ip1[16],ip2[16];
    
    a.toString(ip1);
    b.toString(ip2);
    
    XCTAssertEqual(strcmp(ip1,"127.0.0.1"), 0);
    XCTAssertEqual(strcmp(ip2,s), 0);
}
@end
