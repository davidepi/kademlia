#include "../src/Ip.hpp"
#define XCTEST



#include <gtest/gtest.h>







TEST(Ip,creationEmpty)
{
    Ip a;
    EXPECT_EQ(a.getIp(), 0x100007F);
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
    EXPECT_EQ(a.getIp(), i1);
    EXPECT_EQ(b.getIp(), i2);
    EXPECT_EQ(c.getIp(), i3);
    EXPECT_EQ(d.getIp(), i4);
}

TEST(Ip,Localhost)
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");

    EXPECT_EQ(a.getIp(), 0x100007F);
    EXPECT_EQ(b.getIp(), 0x100007F);
    EXPECT_EQ(c.getIp(), 0x100007F);
}

TEST(Ip,LocalhostFunctionChecking)
{
    Ip a;
    Ip b("127.0.0.1");
    Ip c("localhost");
    Ip d("10.194.245.3");

    EXPECT_TRUE(a.isLocalhost());
    EXPECT_TRUE(b.isLocalhost());
    EXPECT_TRUE(c.isLocalhost());
    EXPECT_FALSE(d.isLocalhost());
}

TEST(Ip,assignOperator)
{
    Ip a;
    Ip b("10.194.30.240");

    a = b;
    EXPECT_EQ(a.getIp(), b.getIp());
}

TEST(Ip,equalOperator)
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(Ip,notEqualOperator)
{
    Ip a;
    Ip b("localhost");
    Ip c("10.145.34.93");

    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a != b);
}

TEST(Ip,toStringOperator)
{
    Ip a;
    char s[] = "10.194.255.255";
    Ip b(s);

    char ip1[16],ip2[16];

    a.toString(ip1);
    b.toString(ip2);

    EXPECT_EQ(strcmp(ip1,"127.0.0.1"), 0);
    EXPECT_EQ(strcmp(ip2,s), 0);
}
