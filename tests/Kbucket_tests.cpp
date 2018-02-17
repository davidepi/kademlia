#include "Kbucket.hpp"
#include "Messenger.hpp"
#define XCTEST



#include <gtest/gtest.h>


TEST(Kbucket,creationEmpty)
{
    Kbucket k;
    EXPECT_TRUE(true);
}

TEST(Kbucket,deletion)
{
    Kbucket* k = new Kbucket();
    delete k;
}

TEST(Kbucker,getSize)
{
    Kbucket k;
    EXPECT_EQ(k.getSize(), 0);

    k.addNode(Node("127.0.0.1",3400));
    EXPECT_EQ(k.getSize(), 1);
}

TEST(Kbucket,addNode)
{
    //empty space and node not present
    Kbucket k;
    Node me("127.0.0.1",3400);
    k.addNode(me);
    for(int i=1;i<KBUCKET_SIZE;i++)
    {
        Node n("127.0.0.1",3400+i);
        k.addNode(n);
    }
    EXPECT_EQ(k.getSize(),KBUCKET_SIZE);

    //add myself
    const std::list<Node>* list = k.getNodes();
    EXPECT_EQ(list->back().getPort(), 3400); //I am the last one
    EXPECT_NE(list->front().getPort(), 3400); //I am not the first one
    k.addNode(me);
    EXPECT_EQ(list->front().getPort(), 3400); //I am the first one
    EXPECT_NE(list->back().getPort(), 3400); //I am not the last one

    //full space and node not present
    std::queue<Message*>* q;    //without Messenger the socket will throw exc.
    Messenger::getInstance().init(q,3400,true);
    Node extra("127.0.0.1",10000);
    k.addNode(extra);
    EXPECT_TRUE(true);//no need to assert presence, it will be a duty of the
                        //updater class
}

TEST(Kbucket,getNodes)
{
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }

    const std::list<Node>* list = k.getNodes();
    int i=KBUCKET_SIZE-1; //used to assert the port, in reverse order
    for(std::list<Node>::const_iterator j=list->begin();j!=list->end();++j)
    {
        EXPECT_EQ(j->getIp(), def);
        EXPECT_EQ(j->getPort(), 3400+i);
        i--;
    }
}

TEST(Kbucket,setNodes)
{
    //add some nodes
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }

    //change node vals
    const std::list<Node>* list = k.getNodes();
    std::list<Node> list_copy = *list;
    int i = 0;
    for(std::list<Node>::iterator j=list_copy.begin();j!=list_copy.end();++j)
    {
        *j = Node(def,6400+i);
        i++;
    }

    //set vals
    k.setNodes(&list_copy);

    //retrieve nodes and assert them
    list = k.getNodes();
    i=0; //used to assert the port, in reverse order
    for(std::list<Node>::const_iterator j=list->begin();j!=list->end();++j)
    {
        EXPECT_EQ(j->getIp(), def);
        EXPECT_EQ(j->getPort(), 6400+i);
        i++;
    }
}

TEST(Kbucket,serialize_deserialize)
{
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }

    uint8_t data[500];
    k.serialize(data);

    //just check the size
    uint16_t node_number = ntohs(((uint16_t*)data)[0]);
    EXPECT_EQ(node_number, KBUCKET_SIZE);

    //craft another bucket
    Kbucket k2(data);

    //ensure the two buckets are identical
    const std::list<Node>* list2 = k2.getNodes();
    //need to transform comparison list into a vector for O(1) access

    int i=0;
    std::vector<Node> compare;
    compare.insert(compare.end(),k.getNodes()->begin(), k.getNodes()->end());
    for(std::list<Node>::const_iterator j=list2->begin();j!=list2->end();++j)
    {
        EXPECT_EQ(j->getIp(),compare.at(i).getIp());
        EXPECT_EQ(j->getPort(),compare.at(i).getPort());
        i++;
    }
}

TEST(Kbucket,contains)
{
#if KBUCKET_SIZE < 5
#warning "Test will search the 5th position in the Kbucket and fail"
#endif

    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }

    //check existing
    Node yes("127.0.0.1",3404);
    EXPECT_TRUE(k.contains(&yes));

    //check non-existing
    Node no("127.0.0.1",3800);
    EXPECT_FALSE(k.contains(&no));
}

TEST(Kbucket,print)
{
    //can't test the print, just trying the run
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    k.print();
}

TEST(Kbucket,delete)
{
    //empty space and node not present
    Kbucket k;
    Node me("127.0.0.1",3400);
    k.addNode(me);
    for(int i=1;i<KBUCKET_SIZE;i++)
    {
        Node n("127.0.0.1",3400+i);
        k.addNode(n);
    }
    EXPECT_EQ(k.getSize(),KBUCKET_SIZE);
    k.deleteNode(Node("127.0.0.1",3404));

    EXPECT_EQ(k.getSize(), KBUCKET_SIZE-1);
}

