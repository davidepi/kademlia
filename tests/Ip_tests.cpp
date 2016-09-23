#include "../src/Ip.hpp"



#include <CppUTest/TestHarness.h>
TEST_GROUP(Ip)
{ };






TEST(Ip,creationEmpty)
{
    Ip a;
    CHECK_EQUAL(a.getIp(), 0x100007F);
}

TEST(Ip,creationGivenNetworkOrdered)
{
    int i1 = 0x64839504;
    int i2 = 0x84759354;
    int i3 = 0x74868658;
    int i4 = 0x01930458;
    Ip a(i1);
    Ip b(i2);
    Ip c(i3);
    Ip d(i4);
    CHECK_EQUAL(a.getIp(), i1);
    CHECK_EQUAL(b.getIp(), i2);
    CHECK_EQUAL(c.getIp(), i3);
    CHECK_EQUAL(d.getIp(), i4);
}

TEST(Ip,Localhost)
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");

    CHECK_EQUAL(a.getIp(), 0x100007F);
    CHECK_EQUAL(b.getIp(), 0x100007F);
    CHECK_EQUAL(c.getIp(), 0x100007F);
}

TEST(Ip,LocalhostFunctionChecking)
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");
    Ip d("10.194.245.3");

    CHECK(a.isLocalhost());
    CHECK(b.isLocalhost());
    CHECK(c.isLocalhost());
    CHECK_FALSE(d.isLocalhost());
}

TEST(Ip,assignOperator)
{
    Ip a;
    Ip b("10.194.30.240");

    a = b;
    CHECK_EQUAL(a.getIp(), b.getIp());
}

TEST(Ip,equalOperator)
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");

    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST(Ip,notEqualOperator)
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");

    CHECK(a != c);
    CHECK_FALSE(a != b);
}

TEST(Ip,toStringOperator)
{
    Ip a;
    char s[] = "10.194.255.255";
    Ip b(s);

    char ip1[16],ip2[16];

    a.toString(ip1);
    b.toString(ip2);

    CHECK_EQUAL(strcmp(ip1,"127.0.0.1"), 0);
    CHECK_EQUAL(strcmp(ip2,s), 0);
}
