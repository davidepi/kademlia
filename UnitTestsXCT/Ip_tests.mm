#import <XCTest/XCTest.h>
#include "Ip.hpp"
#define XCTEST

@interface Ip_tests : XCTestCase

@end

@implementation Ip_tests

- (void)setUp {[super setUp];}

- (void)tearDown {[super tearDown];
}

- (void)test00_Ip_creationEmpty
{
    Ip a;
    XCTAssertEqual(a.getIp(), 0x7F000001);
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
    XCTAssertEqual(a.getIpHo(),ntohl(i1));
    XCTAssertEqual(b.getIpHo(),ntohl(i2));
    XCTAssertEqual(c.getIpHo(),ntohl(i3));
    XCTAssertEqual(d.getIpHo(),ntohl(i4));
}

- (void)test02_Ip_creationGivenString
{
    Ip i0("63.5.206.146");
    Ip i1("252.172.36.85");
    Ip i2("82.242.64.51");
    Ip i3("246.37.169.25");
    Ip i4("223.152.98.234");
    Ip i5("119.73.91.82");
    Ip i6("27.47.156.76");
    Ip i7("11.164.147.119");
    Ip i8("192.36.70.144");
    Ip i9("67.169.13.16");

    XCTAssertEqual(i0.getIp(),0x3F05CE92);
    XCTAssertEqual(i1.getIp(),0xFCAC2455);
    XCTAssertEqual(i2.getIp(),0x52F24033);
    XCTAssertEqual(i3.getIp(),0xF625A919);
    XCTAssertEqual(i4.getIp(),0xDF9862EA);
    XCTAssertEqual(i5.getIp(),0x77495B52);
    XCTAssertEqual(i6.getIp(),0x1B2F9C4C);
    XCTAssertEqual(i7.getIp(),0x0BA49377);
    XCTAssertEqual(i8.getIp(),0xC0244690);
    XCTAssertEqual(i9.getIp(),0x43A90D10);
}

- (void)test03_Ip_Localhost
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");
    
    XCTAssertEqual(a.getIp(), 0x7F000001);
    XCTAssertEqual(b.getIp(), 0x7F000001);
    XCTAssertEqual(c.getIp(), 0x7F000001);
}

-(void)test04_Ip_LocalhostFunctionChecking
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

- (void)test05_Ip_assignOperator
{
    Ip a;
    Ip b("10.194.30.240");
    
    a = b;
    XCTAssertEqual(a.getIp(), b.getIp());
}

- (void)test06_Ip_equalOperator
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");
    
    XCTAssertTrue(a == b);
    XCTAssertFalse(a == c);
}

- (void)test07_Ip_notEqualOperator
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");
    
    XCTAssertTrue(a != c);
    XCTAssertFalse(a != b);
}

- (void)test08_Ip_toStringOperator
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

- (void)test09_Ip_isPrivate
{
    Ip i1("127.0.0.1");
    Ip i2("10.0.0.10");
    Ip i3("10.255.255.255");
    Ip i4("192.168.0.2");
    Ip i5("192.168.1.2");
    Ip i6("192.167.1.2");
    Ip i7("172.16.10.4");
    Ip i8("171.16.10.4");
    Ip i9("169.254.3.3");
    Ip i10("169.255.3.3.");
    Ip i11("8.8.8.8");
    Ip i12("8.8.4.4");
    Ip i13("126.0.0.1");
    Ip i14("72.41.253.84");
    Ip i15("172.15.10.4");
    Ip i16("172.31.10.4");
    Ip i17("172.32.10.4");
    
    XCTAssertTrue(i1.isPrivate());
    XCTAssertTrue(i2.isPrivate());
    XCTAssertTrue(i3.isPrivate());
    XCTAssertTrue(i4.isPrivate());
    XCTAssertTrue(i5.isPrivate());
    XCTAssertFalse(i6.isPrivate());
    XCTAssertTrue(i7.isPrivate());
    XCTAssertFalse(i8.isPrivate());
    XCTAssertTrue(i9.isPrivate());
    XCTAssertFalse(i10.isPrivate());
    XCTAssertFalse(i11.isPrivate());
    XCTAssertFalse(i12.isPrivate());
    XCTAssertFalse(i13.isPrivate());
    XCTAssertFalse(i14.isPrivate());
    XCTAssertFalse(i15.isPrivate());
    XCTAssertTrue(i16.isPrivate());
    XCTAssertFalse(i17.isPrivate());
}
@end
