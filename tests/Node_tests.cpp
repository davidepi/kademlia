#include "../src/Node.hpp"



#include <CppUTest/TestHarness.h>
TEST_GROUP(Node)
{ };






TEST(Node,creationEmpty)
{
    Node n;
    const Key* k = n.getKey();
    CHECK(k == NULL);
}

TEST(Node,creationIpPort)
{
    char originalip[] = "10.196.18.24";
    int originalport = 3400;
    Node n(originalip,originalport);
    char ip[16];
    n.getIp().toString(ip);
    CHECK_EQUAL(strcmp(ip,originalip), 0);
    CHECK_EQUAL(n.getPort(), originalport);
}

TEST(Node,opEqual)
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);

    CHECK(a == b);
    CHECK(a == c);
    CHECK_FALSE(a == d);
    CHECK_FALSE(a == e);
    CHECK_FALSE(a == f);

}

TEST(Node,opNotEqual)
{
    Node a("localhost",3400);
    Node b("127.0.0.1",3400);
    Node c("127.0.0.1",3400);
    Node d("10.196.190.10",3400);
    Node e("127.0.0.1",3500);
    Node f ("10.196.1.1",3500);

    CHECK_FALSE(a != b);
    CHECK_FALSE(a != c);
    CHECK(a != d);
    CHECK(a != e);
    CHECK(a != f);
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

    CHECK(n == n2);
    CHECK(n == n3);
    CHECK(n2 == n3);

    CHECK_EQUAL(n.getKey(), n2.getKey());
    CHECK_EQUAL(n.getKey(), n3.getKey());
    CHECK_EQUAL(n2.getKey(), n3.getKey());

    CHECK(n.getKey()[0] == n2.getKey()[0]);
    CHECK(n.getKey()[0] == n3.getKey()[0]);
    CHECK(n2.getKey()[0] == n3.getKey()[0]);

    n = n3;
}

