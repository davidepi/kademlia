#import <XCTest/XCTest.h>
#include "Distance.hpp"

@interface Distance_tests : XCTestCase

@end

@implementation Distance_tests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)test00_Distance_SameKey
{
    Key a("key1"); //0xb299ff9e....
    Distance d1(a,a);
    
    XCTAssertEqual(d1.getDistance(), -1);
}

-(void)test01_Distance_SameNode
{
    Node a("127.0.0.1",3400);
    Distance d1(a,a);
    
    XCTAssertEqual(d1.getDistance(), -1);
}

-(void)test02_Distance_TwoKeys
{
    Key a("127.0.0.1");
    Key b("10.196.10.4");
    
    XCTAssertGreaterThanOrEqual(Distance(a,b).getDistance(), 0);
}

-(void)test03_Distance_TwoNodes
{
    Node a("127.0.0.1",3400);
    Node b("10.196.10.4",65000);
    
    XCTAssertGreaterThanOrEqual(Distance(a,b).getDistance(), 0);
}

-(void)test04_Distance_getDistanceExactly1
{
    
    Key a("HaBfqvbOBdDBvWzdHxgz");
    Key b("3qrtaITKnny3rhOfxEtE");

    XCTAssertEqual(Distance(a,b).getDistance(), 0);
}

-(void)test05_Distance_getDistanceExactly2
{
    
    Key a("Nd2UPRGG89tTXLBFdTM0");
    Key b("ZJR0aWFdJUnWV1cUBvtq");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 1);
}

-(void)test06_Distance_getDistanceExactly3
{
    
    Key a("UWG3PvZZTeMMauloKm59");
    Key b("Yaf1VdrZMSxHUJU1Nsdt");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 2);
}

-(void)test07_Distance_getDistanceExactly4
{
    
    Key a("jQnjrSBhZBlNWGl2WBC8");
    Key b("9mbg2sSSHCk2Le10Dt16");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 3);
}

-(void)test08_Distance_getDistanceExactly5
{
    
    Key a("RUXX9tK2i9nhAYn0N2fJ");
    Key b("4FnegdVPduGLY54O5O4R");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 4);
}

-(void)test09_Distance_getDistanceExactly6
{
    
    Key a("6pgabi9UOuSpM2Tlh01v");
    Key b("OzwvSikHFswuzleht6xG");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 5);
}

-(void)test10_Distance_getDistanceExactly7
{
    
    Key a("u0G1JuvMJqw0tOHPAwmL");
    Key b("iIGjRgs2Rp5t0cRLGBXR");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 6);
}

-(void)test11_Distance_getDistanceExactly8
{
    
    Key a("x40P55LKc2l8QBvC0H9M");
    Key b("RDDqnUQ5HKch7RmztXsa");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 7);
}

- (void)test12_Distance_getDistanceExaclty2MultipleValues
{
    Key a("AUNYD4iaddbXeeo8vwsH");
    Key b("upNxt1RLw3vMGQThDuAE");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 2);
}

- (void)test13_Distance_getDistanceSecondByte
{
    Key a("Ebu87Kn8nfOjhSxpYPaX");
    Key b("QRLZu7w3KUDP9bxgHPic");
    
    XCTAssertEqual(Distance(a,b).getDistance(),8);
}

- (void)test14_Distance_opLessThan_firstTrue
{
    Key a("GbhLKrs1BqjbkcKd2fiw");
    Key b("V1HIST1r23It98FtD7Ev");
    Key c("9st9v7VSK5ta3BFcqMqT");
    Key d("pYVhfk81O5mTWrvOGqqL");
    
    XCTAssertTrue(Distance(a,b) < Distance(c,d));
}

- (void)test15_Distance_opLessThan_secondTrue
{
   Key a("OWvU6xxcBPp9Ml52CJxw");
   Key b("J5rV89VL9pLBSHecIeyN");
   Key c("WmadfAYyFWFL5CoY5J42");
   Key d("QX659NlqHABReLgSWDwZ");
    
    XCTAssertTrue((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    XCTAssertTrue(Distance(a,b) < Distance(c,d));
}

-(void)test16_Distance_opLessThan_false
{
    Key a("4pkqRFkElfZqybGpLW4j");
    Key b("lNwY926r33eRhBvggOTz");
    Key c("tz4A1RpsHbP95Ig8jCiR");
    Key d("OCDSg5ukGzwgsSXeCz9m");
    
    XCTAssertFalse(Distance(a,b) < Distance(c,d));
}

- (void)test17_Distance_opGreaterThan_firstTrue
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    Key d("OpcBLot6xtl97pGZIPtR");
    
    XCTAssertTrue(Distance(a,b) > Distance(c,d));
}

- (void)test18_Distance_opGreaterThan_secondTrue
{
    Key a("XqtsC633kSQB7v0FJ4Rh");
    Key b("r7mZZ8n86HaPZXkjRwjP");
    Key c("gMWf3IWpzH0UA9sqYOLu");
    Key d("e9I6eiDsDmVg85jbg0AA");
    
    XCTAssertTrue(Distance(a,b) > Distance(c,d));
}

- (void)test19_Distance_opGreaterThan_false
{
    Key a("Nqowc9eWwpXxBrbePjY8");
    Key b("y82FUInSXBqNrmXEfeu5");
    Key c("ApRYAbv6RgKtLaibOsar");
    Key d("KwkyLDM4mN7BBGs6HRcB");
    
    XCTAssertFalse(Distance(a,b) > Distance(c,d));
}

- (void)test20_Distance_opLessThanOrEqual
{
    Key a("GbhLKrs1BqjbkcKd2fiw");
    Key b("V1HIST1r23It98FtD7Ev");
    Key c("9st9v7VSK5ta3BFcqMqT");
    Key d("pYVhfk81O5mTWrvOGqqL");
    
    XCTAssertTrue(Distance(a,b) <= Distance(c,d));
    XCTAssertTrue(Distance(a,b) <= Distance(a,b));
    XCTAssertTrue(Distance(c,d) <= Distance(c,d));
}

- (void)test21_Distance_opGreaterThanOrEqual
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    Key d("OpcBLot6xtl97pGZIPtR");
    
    XCTAssertTrue(Distance(a,b) >= Distance(c,d));
    XCTAssertTrue(Distance(a,b) >= Distance(a,b));
    XCTAssertTrue(Distance(c,d) >= Distance(c,d));
}

- (void)test22_Distance_opEqual
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    XCTAssertTrue(Distance(a,b) == Distance(a,b));
    XCTAssertFalse(Distance(a,b) == Distance(a,c));
}

- (void)test23_Distance_opNotEqual
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    XCTAssertFalse(Distance(a,b) != Distance(a,b));
    XCTAssertTrue(Distance(a,b) != Distance(a,c));
}

@end
