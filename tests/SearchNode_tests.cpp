#include "SearchNode.hpp"
#include <ctime>
#include <cstdlib>

#define XCTEST



#include <gtest/gtest.h>






TEST(SearchNode,insertionDistanceDesc)
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
        k.addNode(askme);
    }
    SearchNode sc(findme,&k);

    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,insertionDistanceAsc)
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
        k.addNode(askme);
    }
    SearchNode sc(findme,&k);

    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,insertionDistanceRand)
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<ALPHA_REQUESTS)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);

    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,getActive)
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
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    EXPECT_EQ(sc.getActive(),0);
    EXPECT_EQ(sc.getUnknown(), ALPHA_REQUESTS);
    sc.addAnswer(askme, &k);
    EXPECT_EQ(sc.getActive(), 1);
}

TEST(SearchNode,importanceOrdering)
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
            k.addNode(askme);
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
            closer.addNode(askme);
        i=closer.getNodes()->size();
    }

    Node res[ALPHA_REQUESTS];
    sc.queryTo(res);
    EXPECT_EQ(sc.getPending(), ALPHA_REQUESTS);
    sc.addAnswer(res[2],&closer);
    EXPECT_EQ(sc.getUnknown(),KBUCKET_SIZE-1);
    EXPECT_EQ(sc.getActive(), 1);
}

TEST(SearchNode,nonDuplication)
{
    //generate some nodes with max distance
    //set some of them as pending
    //add 1 node closer than the others more than KBUCKET_SIZE times
    //ensure that the pending nodes are still there
    srand((unsigned int)time(NULL));

    //otherwise the test will fail
    EXPECT_LT(ALPHA_REQUESTS, KBUCKET_SIZE/2);

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
            k.addNode(askme);
        }
        else if(distance==NBYTE*8-2 && closer_added<KBUCKET_SIZE/2)
        {
            closer_added++;
            closer.addNode(askme);
        }
        else
            continue;
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);

    //generate some pending nodes
    Node res[ALPHA_REQUESTS];
    sc.queryTo(res);
    EXPECT_EQ(sc.getPending(), ALPHA_REQUESTS);
    EXPECT_EQ(sc.getUnknown(), KBUCKET_SIZE-sc.getPending());

    //continuously add the closer ones
    for(int i=0;i<KBUCKET_SIZE+1;i++)
        sc.addAnswer(res[0], &closer);

    EXPECT_EQ(sc.getPending(), ALPHA_REQUESTS-1);
    EXPECT_EQ(sc.getActive(), 1);
}

TEST(SearchNode,nonFull)
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<ALPHA_REQUESTS)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    Node res[ALPHA_REQUESTS];
    EXPECT_EQ(sc.queryTo(res),ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS;i++)
    {
        sc.addAnswer(res[i], &k);
    }
    EXPECT_EQ(sc.queryTo(res), 0);
    EXPECT_EQ(sc.getUnknown(), 0);
    EXPECT_EQ(sc.getPending(), 0);
    EXPECT_EQ(sc.getActive(), ALPHA_REQUESTS);
}

TEST(SearchNode,fullKbucketInsertion)
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);

    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,fullKbucketInsertionKey)
{
    //add
    srand((unsigned int)time(NULL));
    Node findme(Ip(rand()),rand()%65536);
    Kbucket k;
    long i=k.getNodes()->size();
    while(i<KBUCKET_SIZE)
    {
        Node askme(Ip((rand()%0xFFFFFFFF)),rand()%65536);;
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme.getKey(),&k);

    //retrieve
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,sequentialInsertion)
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
        k.addNode(requester[i]);
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
            k.addNode(addme);
        }
        sc.addAnswer(requester[i], &k);
    }

    //check answers
    Node res[ALPHA_REQUESTS];
    int retval = sc.queryTo(res);

    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,sequentialInsertionWithDuplicates)
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
        k.addNode(requester[i]);
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
            k.addNode(addme);
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
        k2.addNode(res[i]);
    }
    //fill remaining kbucket
    for(int i=0;i<KBUCKET_SIZE-ALPHA_REQUESTS;i++)
    {
        Node addme(Ip(rand()),rand()%65536);
        k2.addNode(addme);
    }
    sc.addAnswer(res[0], &k2);

    retval = sc.queryTo(res);
    EXPECT_EQ(retval, ALPHA_REQUESTS);
    for(int i=0;i<ALPHA_REQUESTS-1;i++)
    {
        int dis1 = Distance(res[i],findme).getDistance();
        int dis2 = Distance(res[i+1],findme).getDistance();
        EXPECT_LE(dis1,dis2);
    }
}

TEST(SearchNode,everythingQueriedButPending)
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
        k.addNode(requester[i]);
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
            k.addNode(addme);
        }
        sc.addAnswer(requester[i], &k);
    }

    //check answers
    Node res[ALPHA_REQUESTS];
    int hardterminator = 10000; //kill the test in case of bug
    while(sc.getUnknown()>3 && hardterminator>0)
    {
        EXPECT_EQ(sc.queryTo(res),ALPHA_REQUESTS);
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            EXPECT_LE(dis1,dis2);
        }
        hardterminator--;
    }
    EXPECT_NE(hardterminator, 0);
    //gtest executes the assertEqual in undefined order
    int unknown_before = sc.getUnknown();
    EXPECT_EQ(unknown_before,sc.queryTo(res));
    EXPECT_EQ(sc.getUnknown(), 0);
    EXPECT_EQ(sc.queryTo(res),-1);
    EXPECT_EQ(sc.getPending(), KBUCKET_SIZE-sc.getActive());
}

TEST(SearchNode,everythingQueriedAndCompleted)
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
        k.addNode(requester[i]);
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
            k.addNode(addme);
        }
        sc.addAnswer(requester[i], &k);
    }

    //generate default answer
    Kbucket ke;
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node addme(Ip(rand()),rand()%65536);
        ke.addNode(addme);
    }

    //ask nodes, check if they are ordered and insert them
    int hardterminator = 10000;
    Node res[ALPHA_REQUESTS];
    while(sc.getUnknown()>3 && hardterminator>0)
    {
        EXPECT_EQ(sc.queryTo(res),ALPHA_REQUESTS);
        for(int i=0;i<ALPHA_REQUESTS-1;i++)
        {
            int dis1 = Distance(res[i],findme).getDistance();
            int dis2 = Distance(res[i+1],findme).getDistance();
            EXPECT_LE(dis1,dis2);
            sc.addAnswer(res[i], &ke);
        }
        sc.addAnswer(res[ALPHA_REQUESTS-1], &ke);
        hardterminator--;
    }
    EXPECT_GT(hardterminator, 0);
    EXPECT_LT(sc.getUnknown(), sc.getActive());
    int unknown_before = sc.getUnknown();
    EXPECT_EQ(unknown_before, sc.queryTo(res));
    for(int i=0;i<ALPHA_REQUESTS;i++)
        sc.addAnswer(res[i], &ke);
    EXPECT_EQ(sc.getActive(), KBUCKET_SIZE);
    EXPECT_EQ(sc.queryTo(res), 0);
}

TEST(SearchNode,fullExecution)
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
        k.addNode(requester[i]);
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
            k.addNode(addme);
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
            EXPECT_LE(dis1,dis2);

            //generate answer
            Kbucket k;
            for(int i=0;i<KBUCKET_SIZE;i++)
            {
                Node addme(Ip(rand()),rand()%65536);
                k.addNode(addme);
            }
            sc.addAnswer(res[i], &k);
        }

        //generate answer for the last of the alpha nodes
        Kbucket k;
        for(int i=0;i<KBUCKET_SIZE;i++)
        {
            Node addme(Ip(rand()),rand()%65536);
            k.addNode(addme);
        }
        sc.addAnswer(res[queryres-1], &k);
        hardterminator--;
    }
    EXPECT_GT(hardterminator, 0);
    EXPECT_EQ(sc.queryTo(res), 0);
    EXPECT_EQ(sc.getUnknown(), 0);
    EXPECT_EQ(sc.getPending(),0);
    EXPECT_EQ(sc.getActive(), KBUCKET_SIZE);
    sc.print();
}

TEST(SearchNode,getAnswer)
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
        k.addNode(askme);
        i=k.getNodes()->size();
    }
    SearchNode sc(findme,&k);
    EXPECT_EQ(sc.getActive(),0);
    EXPECT_EQ(sc.getUnknown(), ALPHA_REQUESTS);
    sc.addAnswer(askme, &k);
    sc.getAnswer(&k);
    EXPECT_TRUE(k.contains(&askme));
}

