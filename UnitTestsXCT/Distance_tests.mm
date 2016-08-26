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
    
    XCTAssertGreaterThan(Distance(a,b).getDistance(),0);
}

-(void)test03_Distance_TwoNodes
{
    Node a("127.0.0.1",3400);
    Node b("10.196.10.4",65000);
    
    XCTAssertGreaterThan(Distance(a,b).getDistance(),0);
}


@end
