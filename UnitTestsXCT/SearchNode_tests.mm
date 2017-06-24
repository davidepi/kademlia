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

- (void)test04_SearchNode_getActive
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    Node askme;
    while(i<ALPHA_REQUESTS)
    {
        askme = Node(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.add(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    XCTAssertEqual(sc.getActive(),0);
    XCTAssertEqual(sc.getUnknown(), ALPHA_REQUESTS);
    sc.addAnswer(askme, &k);
    XCTAssertEqual(sc.getActive(), 1);
}

- (void)test06_SearchNode_importanceOrdering
{
    //ensure that active nodes are always before unknown nodes at the same
    //distance
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    Kbucket closer;
    
    //distance KEY-1
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);
        int distance = Distance(*(askme.getKey()),*(findme.getKey())).getDistance();
        if(distance==NBYTE*8-1)
            k.add(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //distance KEY-2
    i=closer.getNodes()->size();
    while(i<KBUCKET_SIZE-1)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);
        int distance = Distance(*(askme.getKey()),*(findme.getKey())).getDistance();
        if(distance==NBYTE*8-2)
            closer.add(askme);
        i=closer.getNodes()->size();
    }
    
    Node res[ALPHA_REQUESTS];
    sc.queryTo(res);
    XCTAssertEqual(sc.getPending(), ALPHA_REQUESTS);
    sc.addAnswer(res[2],&closer);
    XCTAssertEqual(sc.getUnknown(),KBUCKET_SIZE-1);
    XCTAssertEqual(sc.getActive(), 1);
}

- (void)test06_SearchNode_nonDuplication
{
    //generate some nodes with max distance
    //set some of them as pending
    //add 1 node closer than the others more than KBUCKET_SIZE times
    //ensure that the pending nodes are still there
    srand((unsigned int)time(NULL));
    
    //otherwise the test will fail
    XCTAssertLessThan(ALPHA_REQUESTS, KBUCKET_SIZE/2);
    
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    Kbucket closer;
    int closer_added = 0;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE || closer_added<KBUCKET_SIZE/2)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);
        int distance = Distance(*(askme.getKey()),*(findme.getKey())).getDistance();
        if(distance==NBYTE*8-1 && i<KBUCKET_SIZE)
        {
            k.add(askme);
        }
        else if(distance==NBYTE*8-2 && closer_added<KBUCKET_SIZE/2)
        {
            closer_added++;
            closer.add(askme);
        }
        else
            continue;
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    
    //generate some pending nodes
    Node res[ALPHA_REQUESTS];
    sc.queryTo(res);
    XCTAssertEqual(sc.getPending(), ALPHA_REQUESTS);
    XCTAssertEqual(sc.getUnknown(), KBUCKET_SIZE-sc.getPending());
    
    //continuously add the closer ones
    for(int i=0;i<KBUCKET_SIZE+1;i++)
        sc.addAnswer(res[0], &closer);
    
    XCTAssertEqual(sc.getPending(), ALPHA_REQUESTS-1);
    XCTAssertEqual(sc.getActive(), 1);
}

- (void)test05_SearchNode_nonFull
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
    Node res[ALPHA_REQUESTS];
    XCTAssertEqual(sc.queryTo(res),ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        sc.addAnswer(res[i], &k);
    }
    XCTAssertEqual(sc.queryTo(res), 0);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(), 0);
    XCTAssertEqual(sc.getActive(), ALPHA_REQUESTS);
}

- (void)test06_SearchNode_fullKbucketInsertion
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

- (void)test07_SearchNode_fullKbucketInsertionKey
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

- (void)test08_SearchNode_sequentialInsertion
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

- (void)test09_SearchNode_sequentialInsertionWithDuplicates
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

- (void)test10_SearchNode_everythingQueriedButPending
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
    int hardterminator = 10000; //kill the test in case of bug
    while(sc.getUnknown()>3 && hardterminator>0)
    {
        XCTAssertEqual(sc.queryTo(res),ALPHA_REQUESTS);
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
        }
        hardterminator--;
    }
    XCTAssertNotEqual(hardterminator, 0);
    //gtest executes the assertEqual in undefined order
    int unknown_before = sc.getUnknown();
    XCTAssertEqual(unknown_before,sc.queryTo(res));
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.queryTo(res),-1);
    XCTAssertEqual(sc.getPending(), KBUCKET_SIZE-sc.getActive());
}

- (void)test11_SearchNode_everythingQueriedAndCompleted
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
    
    //ask nodes, check if they are ordered and insert them
    int hardterminator = 10000;
    Node res[ALPHA_REQUESTS];
    while(sc.getUnknown()>3 && hardterminator>0)
    {
        XCTAssertEqual(sc.queryTo(res),ALPHA_REQUESTS);
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
            sc.addAnswer(res[i], &ke);
        }
        sc.addAnswer(res[ALPHA_REQUESTS-1], &ke);
        hardterminator--;
    }
    XCTAssertGreaterThan(hardterminator, 0);
    XCTAssertLessThan(sc.getUnknown(), sc.getActive());
    int unknown_before = sc.getUnknown();
    XCTAssertEqual(unknown_before, sc.queryTo(res));
    for(int i=0;i<ALPHA_REQUESTS;i++)
        sc.addAnswer(res[i], &ke);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE);
    XCTAssertEqual(sc.queryTo(res), 0);
}

- (void)test12_SearchNode_fullExecution
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
    
    //ask nodes, check if they are ordered and insert them
    int hardterminator = 10000;
    Node res[ALPHA_REQUESTS];
    int queryres;
    while((queryres = sc.queryTo(res))>0 && hardterminator>0)
    {
        for(int i=0;i<queryres-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
            
            //generate answer
            Kbucket k;
            for(int i=0;i<KBUCKET_SIZE;i++)
            {
                Node addme(Ip(rand()),rand()%65536);
                k.add(addme);
            }
            sc.addAnswer(res[i], &k);
        }
        
        //generate answer for the last of the alpha nodes
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(res[queryres-1], &k);
        hardterminator--;
    }
    XCTAssertGreaterThan(hardterminator, 0);
    XCTAssertEqual(sc.queryTo(res), 0);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(),0);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE);
    sc.print();
}

- (void)test13_SearchNode_nodeEvictionNoReserve
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
    Node res[ALPHA_REQUESTS];
    XCTAssertEqual(sc.queryTo(res),ALPHA_REQUESTS);
    int skipme = rand()%3;
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        if(i!=skipme)
            sc.addAnswer(res[i], &k);
    }
    XCTAssertEqual(sc.queryTo(res), -1);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(), 1);
    XCTAssertEqual(sc.getActive(), ALPHA_REQUESTS-1);
    sc.evict(res[skipme]);
    XCTAssertEqual(sc.queryTo(res), 0);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(), 0);
    XCTAssertEqual(sc.getActive(), ALPHA_REQUESTS-1);
}

- (void)test14_SearchNode_evictionOnlyPendingReserve
{
    srand((unsigned int)time(NULL));
    while(true)
    {
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
    XCTAssertEqual(sc.getUnknown(), KBUCKET_SIZE);
    
    Node res[ALPHA_REQUESTS];
    
    //set everything as pending;
    int hardterminator=10000;
    sc.queryTo(res);
    Node onlygoodone = res[0]; //this is the onlyone that will reply
    while(sc.queryTo(res)&&hardterminator>0)
        hardterminator--;
    XCTAssertEqual(sc.getPending(), KBUCKET_SIZE);
    
    //generate another kbucket and add it, until at least 1 unknown
    //in the list. if the active is finished in the reserve, retry fron beginning
    while(sc.getUnknown()<1)
    {
        Kbucket ke;
        long i=ke.getNodes()->size();
        while(i<KBUCKET_SIZE)
        {
            Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
            ke.add(askme);
            i=ke.getNodes()->size();
        }
        sc.addAnswer(onlygoodone, &ke);
    }
        
        // by consturction, only 1 node can be active.
        //In this way I am sure that it is not ended up in the reserve list
        //At least 1 unknown means that queryTo will set that to pending,
        //the reserve list is filled with pending with at least 1 guaranteed
        //node there
        if(sc.getUnknown()>0 && sc.getActive()>0)
        {
            while(sc.queryTo(res)!=-1);
            XCTAssertEqual(sc.getUnknown(), 0);
            XCTAssertEqual(sc.getActive(), 1);
            XCTAssertEqual(sc.getPending(), KBUCKET_SIZE-1);
            sc.evict(res[0]);
            XCTAssertEqual(sc.getUnknown(), 0);
            XCTAssertEqual(sc.getActive(), 1);
            XCTAssertEqual(sc.getPending(), KBUCKET_SIZE-2);
            break;
        }
        else //the active node finished in the reserve list. restart.
            continue;
    }
}

- (void)test15_SearchNode_fullExecutionButEvictionWithReserve
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
    
    //ask nodes, check if they are ordered and insert them
    int hardterminator = 10000;
    Node res[ALPHA_REQUESTS];
    Node faulty;
    int queryres;
    while((queryres = sc.queryTo(res))>0 && hardterminator>0)
    {
        for(int i=0;i<queryres-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
            
            //generate answer
            Kbucket k;
            for(int i=0;i<KBUCKET_SIZE;i++)
            {
                Node addme(Ip(rand()),rand()%65536);
                k.add(addme);
            }
            sc.addAnswer(res[i], &k);
        }
        
        //generate answer for the last of the alpha nodes
        if(sc.getPending()!=1 || sc.getUnknown()>0)
        {
            Kbucket k;
            for(int i=0;i<KBUCKET_SIZE;i++)
            {
                Node addme(Ip(rand()),rand()%65536);
                k.add(addme);
            }
            sc.addAnswer(res[queryres-1], &k);
        }
        else
        {
            faulty = res[queryres-1];
        }
        hardterminator--;
    }
    XCTAssertGreaterThan(hardterminator, 0);
    XCTAssertEqual(sc.queryTo(res), -1);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(),1);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE-1);
    sc.evict(faulty);
    XCTAssertEqual(sc.queryTo(res), 0);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(),0);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE);
}

- (void)test16_SearchNode_fullExecutionNoEviction
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
    
    //ask nodes, check if they are ordered and insert them
    int hardterminator = 10000;
    Node res[ALPHA_REQUESTS];
    int queryres;
    while((queryres = sc.queryTo(res))>0 && hardterminator>0)
    {
        for(int i=0;i<queryres-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            XCTAssertLessThanOrEqual(dis1,dis2);
            
            //generate answer
            Kbucket k;
            for(int i=0;i<KBUCKET_SIZE;i++)
            {
                Node addme(Ip(rand()),rand()%65536);
                k.add(addme);
            }
            sc.addAnswer(res[i], &k);
        }
        
        //generate answer for the last of the alpha nodes
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.add(addme);
        }
        sc.addAnswer(res[queryres-1], &k);
        hardterminator--;
    }
    XCTAssertGreaterThan(hardterminator, 0);
    XCTAssertEqual(sc.queryTo(res), 0);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(),0);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE);
    sc.evict(res[0]);
    XCTAssertEqual(sc.getUnknown(), 0);
    XCTAssertEqual(sc.getPending(),0);
    XCTAssertEqual(sc.getActive(), KBUCKET_SIZE);
}


@end
