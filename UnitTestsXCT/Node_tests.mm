#import <XCTest/XCTest.h>
#include "Node.hpp"
#define XCTEST

@interface Node_tests : XCTestCase

@end

@implementation Node_tests

- (void)setUp {[super setUp];}

- (void)tearDown {[super tearDown];
}

- (void)test00_Node_creationEmpty
{
    Node n;
    const Key* k = n.getKey();
    Key k2((uint32_t)0,0);
    XCTAssertTrue(*k==k2);
}

- (void)test01_Node_creationIpPort
{
    char originalip[] = "10.196.18.24";
    int originalport = 3400;
    Node n(originalip,originalport);
    char ip[16];
    n.getIp().toString(ip);
    XCTAssertEqual(strcmp(ip,originalip), 0);
    XCTAssertEqual(n.getPort(), originalport);
}

- (void)test02_Node_opEqual
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);
    
    XCTAssertTrue(a == b);
    XCTAssertTrue(a == c);
    XCTAssertFalse(a == d);
    XCTAssertFalse(a == e);
    XCTAssertFalse(a == f);
    
}

- (void)test03_Node_opNotEqual
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);
    
    XCTAssertFalse(a != b);
    XCTAssertFalse(a != c);
    XCTAssertTrue(a != d);
    XCTAssertTrue(a != e);
    XCTAssertTrue(a != f);
}

- (void)test04_Node_ruleOfThree
{
    Node n("10.192.10.1",3400);
    Node n2(n);
    Node n3 = n;
    
    if(true)
        Node n4 = n; //if rule of three is not verified this is deleted
                     //almost instantly and accessing Node::key will
                     //result in a segafault
    
    XCTAssertTrue(n == n2);
    XCTAssertTrue(n == n3);
    XCTAssertTrue(n2 == n3);
    
    XCTAssertEqual(n.getKey(), n2.getKey());
    XCTAssertEqual(n.getKey(), n3.getKey());
    XCTAssertEqual(n2.getKey(), n3.getKey());
    
    XCTAssertTrue(n.getKey()[0] == n2.getKey()[0]);
    XCTAssertTrue(n.getKey()[0] == n3.getKey()[0]);
    XCTAssertTrue(n2.getKey()[0] == n3.getKey()[0]);
    
    n = n3;
}

- (void)test05_Node_isEmpty
{
    Node n;
    XCTAssertTrue(n.isEmpty());
}

- (void)test06_Node_notEmpty
{
    Node n("10.0.0.1", 4593);
    XCTAssertFalse(n.isEmpty());
}

- (void)test07_Node_less
{
    Node n1("10.0.0.1",4593);
    Node n2("127.0.0.1",6500);
    XCTAssertTrue(n1<n2);
}

- (void)test08_Node_ostream
{
    
    Node n("10.0.0.1",3400);
    std::ostringstream stream;
    stream << n;
    std::string str =  stream.str();
    const char* chr = str.c_str();
    XCTAssertEqual(strcmp("10.0.0.1:3400",chr),0);
}

@end
