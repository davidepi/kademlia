#include "Node.hpp"
#define XCTEST



#include <gtest/gtest.h>







TEST(Node,creationEmpty)
{
    Node n;
    const Key* k = n.getKey();
    Key k2((uint32_t)0,0);
    EXPECT_TRUE(*k==k2);
}

TEST(Node,creationIpPort)
{
    char originalip[] = "10.196.18.24";
    int originalport = 3400;
    Node n(originalip,originalport);
    char ip[16];
    n.getIp().toString(ip);
    EXPECT_EQ(strcmp(ip,originalip), 0);
    EXPECT_EQ(n.getPort(), originalport);
}

TEST(Node,opEqual)
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
    EXPECT_FALSE(a == d);
    EXPECT_FALSE(a == e);
    EXPECT_FALSE(a == f);

}

TEST(Node,opNotEqual)
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);

    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a != c);
    EXPECT_TRUE(a != d);
    EXPECT_TRUE(a != e);
    EXPECT_TRUE(a != f);
}

TEST(Node,ruleOfThree)
{
    Node n("10.192.10.1",3400);
    Node n2(n);
    Node n3 = n;

    if(true)
        Node n4 = n; //if rule of three is not verified this is deleted
                     //almost instantly and accessing Node::key will
                     //result in a segafault

    EXPECT_TRUE(n == n2);
    EXPECT_TRUE(n == n3);
    EXPECT_TRUE(n2 == n3);

    EXPECT_EQ(n.getKey(), n2.getKey());
    EXPECT_EQ(n.getKey(), n3.getKey());
    EXPECT_EQ(n2.getKey(), n3.getKey());

    EXPECT_TRUE(n.getKey()[0] == n2.getKey()[0]);
    EXPECT_TRUE(n.getKey()[0] == n3.getKey()[0]);
    EXPECT_TRUE(n2.getKey()[0] == n3.getKey()[0]);

    n = n3;
}

TEST(Node,isEmpty)
{
    Node n;
    EXPECT_TRUE(n.isEmpty());
}

TEST(Node,notEmpty)
{
    Node n("10.0.0.1", 4593);
    EXPECT_FALSE(n.isEmpty());
}

TEST(Node,less)
{
    Node n1("10.0.0.1",4593);
    Node n2("127.0.0.1",6500);
    EXPECT_TRUE(n1<n2);
}

TEST(Node,ostream)
{

    Node n("10.0.0.1",3400);
    std::ostringstream stream;
    stream << n;
    std::string str =  stream.str();
    const char* chr = str.c_str();
    EXPECT_EQ(strcmp("10.0.0.1:3400",chr),0);
}

