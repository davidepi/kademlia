#import <XCTest/XCTest.h>
#include "SearchNode.hpp"
#include <ctime>
#include <cstdlib>

#define XCTEST

@interface SearchNode_tests : XCTestCase

@end

@implementation SearchNode_tests

- (void)setUp {[super setUp];}

- (void)tearDown {[super tearDown];}

- (void)test01_SearchNode_insertionDistanceDesc
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    unsigned int previous_distance = 0xFFFFFFFF;
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        int dist;
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        do
        {
            askme = Node(Ip(rand()%0xFFFFFFFF),rand()%65536);
            dist = Distance(askme,findme).getDistance();
        }while(dist >= previous_distance || dist < ALPHA_REQUESTS-i);
        previous_distance = dist;
        k.add(askme);
    }
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        XCTAssertLessThanOrEqual(dis1,dis2);
    }
}

- (void)test02_SearchNode_insertionDistanceAsc
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    unsigned int previous_distance = 0;
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        int dist;
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        do
        {
            askme = Node(Ip(rand()%0xFFFFFFFF),rand()%65536);
            dist = Distance(askme,findme).getDistance();
        }while(dist <= previous_distance);
        previous_distance = dist;
        k.add(askme);
    }
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        XCTAssertLessThanOrEqual(dis1,dis2);
    }
}

- (void)test03_SearchNode_insertionDistanceRand
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<ALPHA_REQUESTS)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        XCTAssertLessThanOrEqual(dis1,dis2);
    }
}

- (void)test04_SearchNode_fullKbucketInsertion
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        XCTAssertLessThanOrEqual(dis1,dis2);
    }
}

- (void)test05_SearchNode_sameNodeInsertion
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        k.add(askme);
    }
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, 1);
    XCTAssertEqual(res[0],askme);
}

- (void)test06_SearchNode_KBucketMerging
{
    const int MERGE_SIZE = 5;
    
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    for(int j=0;j<MERGE_SIZE;j++)
    {
        Kbucket k;
        long i=k.getNodes()->size();
        while(i<KBUCKET_SIZE)
        {
            Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
            k.add(askme);
            i=k.getNodes()->size();
        }
        sc.addAnswer(&k);
    }
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    for(int i=0;i<(KBUCKET_SIZE*MERGE_SIZE/ALPHA_REQUESTS);i++)
    {
        int retval = sc.queryTo(res);
        XCTAssertEqual(retval, ALPHA_REQUESTS);
        for(int j=0;j<ALPHA_REQUESTS-1;j++)
        {
            int dis1 = Distance(res[j],findme).getDistance();
            int dis2 = Distance(res[j+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
        }
    }
}

- (void)test07_SearchNode_foundNode
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE-1)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    k.add(findme);
    sc.addAnswer(&k);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, 0);
    XCTAssertEqual(res[0],findme);
}

- (void)test08_SearchNode_tryingToAddAfterNodeFound
{
    //add
    srand(time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    SearchNode sc(findme);
    Kbucket k;
    k.add(findme);
    sc.addAnswer(&k);
    Kbucket g;
    long i=g.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        g.add(askme);
        i=g.getNodes()->size();
    }
    sc.addAnswer(&g);
    
    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    XCTAssertEqual(retval, 0);
    XCTAssertEqual(res[0],findme);
}


@end
