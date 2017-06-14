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
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
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
    SearchNode sc(findme,&k);
    
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
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        int dist;
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        do
        {
            askme = Node(Ip(rand()%0xFFFFFFFF),rand()%65536);
            dist = Distance(askme,findme).getDistance();
        }while(dist != NBYTE*8-(i+1));
        k.add(askme);
    }
    SearchNode sc(findme,&k);
    
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
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<ALPHA_REQUESTS)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
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
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
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

- (void)test05_SearchNode_fullKbucketInsertionKey
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme.getKey(),&k);
    
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

- (void)test06_SearchNode_sequentialInsertion
{
    //generate first answer from my kbucket
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    Node requester[ALPHA_REQUESTS];
    while(i<ALPHA_REQUESTS)
    {
        requester[i] = Node(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(requester[i]);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //generate anwer from the first three pinged nodes
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(requester[i], &k);
    }
    
    //check answers
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

- (void)test06_SearchNode_sequentialInsertionWithDuplicates
{
    //generate first answer from my kbucket
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    Node requester[ALPHA_REQUESTS];
    while(i<ALPHA_REQUESTS)
    {
        requester[i] = Node(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(requester[i]);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //generate answer from the first three pinged nodes
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(requester[i], &k);
    }
    
    //check answers
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);
    
    
    //add some duplicates to kbucket
    Kbucket k2;
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        k2.add(res[i]);
    }
    //fill remaining kbucket
    for(int i=0;i<KBUCKET_SIZE-ALPHA_REQUESTS;i++)
    {
        Node addme(Ip(rand()),rand()%65536);
        k2.add(addme);
    }
    sc.addAnswer(res[0], &k2);
    
    retval = sc.queryTo(res);
    XCTAssertEqual(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        XCTAssertLessThanOrEqual(dis1,dis2);
    }
}

- (void)test07_SearchNode_everythingQueriedButPending
{
    //generate first answer from my kbucket
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    Node requester[ALPHA_REQUESTS];
    while(i<ALPHA_REQUESTS)
    {
        requester[i] = Node(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(requester[i]);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //generate answer from the first three pinged nodes
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(requester[i], &k);
    }
    
    //check answers
    Node res[ALPHA_REQUESTS];
    for(int i=0;i<KBUCKET_SIZE/ALPHA_REQUESTS;i++)
    {
        XCTAssertEqual(sc.queryTo(res),ALPHA_REQUESTS);
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
        }
    }
    XCTAssertEqual(sc.queryTo(res),2);
    XCTAssertEqual(sc.queryTo(res),-1);
}

- (void)test07_SearchNode_everythingQueriedAndCompleted
{
    //generate first answer from my kbucket
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    Node requester[ALPHA_REQUESTS];
    while(i<ALPHA_REQUESTS)
    {
        requester[i] = Node(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(requester[i]);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //generate answer from the first three pinged nodes
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(requester[i], &k);
    }
    
    
    //generate default answer
    Kbucket ke;
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node addme(Ip(rand()),rand()%65536);
        ke.add(addme);
    }
    
    //check answers
    Node res[ALPHA_REQUESTS];
    while(sc.queryTo(res)!=0)
    {
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
        }
        
    }
}

@end
