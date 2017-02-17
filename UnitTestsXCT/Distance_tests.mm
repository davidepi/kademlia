#import <XCTest/XCTest.h>
#include "../src/Distance.hpp"
#define XCTEST
#define XCTEST
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
    
    XCTAssertEqual(d1.getDistance(), 0);
}

-(void)test01_Distance_SameNode
{
    Node a("127.0.0.1",3400);
    Distance d1(a,a);
    
    XCTAssertEqual(d1.getDistance(), 0);
}

-(void)test02_Distance_TwoKeys
{
    Key a("127.0.0.1");
    Key b("10.196.10.4");
    
    XCTAssertTrue(Distance(a,b).getDistance() >= 0);
}

-(void)test03_Distance_TwoNodes
{
    Node a("127.0.0.1",3400);
    Node b("10.196.10.4",65000);
    
    XCTAssertTrue(Distance(a,b).getDistance() >= 0);
}

-(void)test04_Distance_getDistanceExactly1
{
    
    Key a("HaBfqvbOBdDBvWzdHxgz");
    Key b("3qrtaITKnny3rhOfxEtE");

    XCTAssertEqual(Distance(a,b).getDistance(), 1);
}

-(void)test05_Distance_getDistanceExactly2
{
    
    Key a("Nd2UPRGG89tTXLBFdTM0");
    Key b("ZJR0aWFdJUnWV1cUBvtq");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 2);
}

-(void)test06_Distance_getDistanceExactly3
{
    
    Key a("UWG3PvZZTeMMauloKm59");
    Key b("Yaf1VdrZMSxHUJU1Nsdt");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 3);
}

-(void)test07_Distance_getDistanceExactly4
{
    
    Key a("jQnjrSBhZBlNWGl2WBC8");
    Key b("9mbg2sSSHCk2Le10Dt16");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 4);
}

-(void)test08_Distance_getDistanceExactly5
{
    
    Key a("RUXX9tK2i9nhAYn0N2fJ");
    Key b("4FnegdVPduGLY54O5O4R");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 5);
}

-(void)test09_Distance_getDistanceExactly6
{
    
    Key a("6pgabi9UOuSpM2Tlh01v");
    Key b("OzwvSikHFswuzleht6xG");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 6);
}

-(void)test10_Distance_getDistanceExactly7
{
    
    Key a("u0G1JuvMJqw0tOHPAwmL");
    Key b("iIGjRgs2Rp5t0cRLGBXR");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 7);
}

-(void)test11_Distance_getDistanceExactly8
{
    
    Key a("x40P55LKc2l8QBvC0H9M");
    Key b("RDDqnUQ5HKch7RmztXsa");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 8);
}

- (void)test12_Distance_getDistanceExaclty3MultipleValues
{
    Key a("AUNYD4iaddbXeeo8vwsH");
    Key b("upNxt1RLw3vMGQThDuAE");
    
    XCTAssertEqual(Distance(a,b).getDistance(), 3);
}

- (void)test13_Distance_getDistanceSecondByte
{
    Key a("Ebu87Kn8nfOjhSxpYPaX");
    Key b("QRLZu7w3KUDP9bxgHPic");
    
    XCTAssertEqual(Distance(a,b).getDistance(),9);
}

- (void)test14_Distance_opLessThan_firstTrue
{
    Key a("9st9v7VSK5ta3BFcqMqT");
    Key b("pYVhfk81O5mTWrvOGqqL");
    Key c("GbhLKrs1BqjbkcKd2fiw");
    Key d("V1HIST1r23It98FtD7Ev");
    
    XCTAssertTrue(Distance(a,b) < Distance(c,d));
}

- (void)test15_Distance_opLessThan_secondTrue
{
    Key a("WmadfAYyFWFL5CoY5J42");  //0xCA 0x06
    Key b("QX659NlqHABReLgSWDwZ");  //0xCA 0xDA
    Key c("OWvU6xxcBPp9Ml52CJxw");  //0x65 0xC3
    Key d("J5rV89VL9pLBSHecIeyN");  //0x65 0xB6
    
    XCTAssertTrue((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    XCTAssertTrue(Distance(a,b) < Distance(c,d));
}

-(void)test16_Distance_opLessThan_false
{
    Key a("tz4A1RpsHbP95Ig8jCiR");
    Key b("OCDSg5ukGzwgsSXeCz9m");
    Key c("4pkqRFkElfZqybGpLW4j");
    Key d("lNwY926r33eRhBvggOTz");
    
    XCTAssertFalse(Distance(a,b) < Distance(c,d));
}

- (void)test17_Distance_opGreaterThan_firstTrue
{
    Key a("NyZO9NhCcluT0ueB5jwC");
    Key b("OpcBLot6xtl97pGZIPtR");
    Key c("2Tv8cForS9J16ngkrpF9");
    Key d("TqF3AFes5UniXGzhbPiX");
    
    XCTAssertTrue(Distance(a,b) > Distance(c,d));
}

- (void)test18_Distance_opGreaterThan_secondTrue
{
    Key a("gMWf3IWpzH0UA9sqYOLu");
    Key b("e9I6eiDsDmVg85jbg0AA");
    Key c("XqtsC633kSQB7v0FJ4Rh");
    Key d("r7mZZ8n86HaPZXkjRwjP");
    
    XCTAssertTrue((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    
    XCTAssertTrue(Distance(a,b) > Distance(c,d));
}

- (void)test19_Distance_opGreaterThan_false
{
    Key a("ApRYAbv6RgKtLaibOsar");
    Key b("KwkyLDM4mN7BBGs6HRcB");
    Key c("Nqowc9eWwpXxBrbePjY8");
    Key d("y82FUInSXBqNrmXEfeu5");
    
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
    XCTAssertTrue(Distance(a,b) >= Distance(a,b));
    XCTAssertTrue(Distance(a,b) <= Distance(a,b));
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

- (void)test24_Distance_printDistanceSame
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    
    char out1[161];
    char out2[161];
    Distance(a,b).printDistance(out1);
    Distance(a,b).printDistance(out2);
    
    XCTAssertTrue(strcmp(out1,out2)==0);
}

- (void)test24_Distance_printDistanceZero
{
    Key a("NyZO9NhCcluT0ueB5jwC");
    
    char out1[161];
    char out2[161];
    Distance(a,a).printDistance(out1);
    strcpy(out2,"0000000000000000000000000000000000000000");
    strcat(out2,"0000000000000000000000000000000000000000");
    strcat(out2,"0000000000000000000000000000000000000000");
    strcat(out2,"0000000000000000000000000000000000000000");
    
    XCTAssertTrue(strcmp(out1,out2)==0);
}

- (void)test25_Distance_printFixedInputs
{
    Key a("ygLco4MF9PwbGxviiZ6y");
    Key b("6m8CnZsYbNNV7d2wpRep");
    Key c("lVRPeBoopsKPETBfIIXH");
    Key d("pWtqntqKFh2KwUsk9GnH");
    Key e("nNfoelJWY4nEmgCRq60O");
    Key f("WENZjN7wiUJctEpKIN6C");
    Key g("hK2jO7I93jpg7SGxJDrU");
    Key h("ESMoN9aOT14T2X8bNO85");
    Key i("7mP79ZNOVmNwApw1e1HQ");
    Key l("C3RpDVaaWLOViSTcIqUz");
}

@end
