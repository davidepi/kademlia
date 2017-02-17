#import <XCTest/XCTest.h>
#include "../src/Kbucket.hpp"
#define XCTEST

@interface KBucket_tests : XCTestCase

@end

@implementation KBucket_tests

- (void)setUp {[super setUp];}

- (void)tearDown {[super tearDown];
}

- (void)test00_KBucket_creationEmpty
{
    Kbucket k;
    
    XCTAssertTrue(true);
}

- (void)test01_KBucket_deletion
{
    Kbucket* k = new Kbucket();
    delete k;
}

- (void)test01_KBucket_addNodeEmptySpace
{
    Kbucket k;
    Node n("127.0.0.1",3400);
    
    XCTAssertTrue(true);
}

- (void)test02_KBucket_addNodeNoSpace
{
    Kbucket k;
    for(int i=-1;i<KBUCKET_SIZE;i++)
    {
        Node* n = new Node("127.0.0.1",i+1025);
        k.add(*n);
    }
    
    XCTAssertTrue(true);
}
@end
