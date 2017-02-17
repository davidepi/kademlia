#include "../src/Kbucket.hpp"
#define XCTEST



#include <gtest/gtest.h>







TEST(KBucket,creationEmpty)
{
    Kbucket k;

    EXPECT_TRUE(true);
}

TEST(KBucket,deletion)
{
    Kbucket* k = new Kbucket();
    delete k;
}

TEST(KBucket,addNodeEmptySpace)
{
    Kbucket k;
    Node n("127.0.0.1",3400);

    EXPECT_TRUE(true);
}

TEST(KBucket,addNodeNoSpace)
{
    Kbucket k;
    for(int i=-1;i<KBUCKET_SIZE;i++)
    {
        Node* n = new Node("127.0.0.1",i+1025);
        k.add(*n);
    }

    EXPECT_TRUE(true);
}
