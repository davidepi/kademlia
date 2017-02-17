#include "../src/Distance.hpp"
#define XCTEST
#define XCTEST


#include <gtest/gtest.h>












TEST(Distance,SameKey)
{
    Key a("key1"); //0xb299ff9e....
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), -1);
}

TEST(Distance,SameNode)
{
    Node a("127.0.0.1",3400);
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), -1);
}

TEST(Distance,TwoKeys)
{
    Key a("127.0.0.1");
    Key b("10.196.10.4");

    EXPECT_TRUE(Distance(a,b).getDistance() >= 0);
}

TEST(Distance,TwoNodes)
{
    Node a("127.0.0.1",3400);
    Node b("10.196.10.4",65000);

    EXPECT_TRUE(Distance(a,b).getDistance() >= 0);
}

TEST(Distance,getDistanceExactly1)
{

    Key a("HaBfqvbOBdDBvWzdHxgz");
    Key b("3qrtaITKnny3rhOfxEtE");

    EXPECT_EQ(Distance(a,b).getDistance(), 0);
}

TEST(Distance,getDistanceExactly2)
{

    Key a("Nd2UPRGG89tTXLBFdTM0");
    Key b("ZJR0aWFdJUnWV1cUBvtq");

    EXPECT_EQ(Distance(a,b).getDistance(), 1);
}

TEST(Distance,getDistanceExactly3)
{

    Key a("UWG3PvZZTeMMauloKm59");
    Key b("Yaf1VdrZMSxHUJU1Nsdt");

    EXPECT_EQ(Distance(a,b).getDistance(), 2);
}

TEST(Distance,getDistanceExactly4)
{

    Key a("jQnjrSBhZBlNWGl2WBC8");
    Key b("9mbg2sSSHCk2Le10Dt16");

    EXPECT_EQ(Distance(a,b).getDistance(), 3);
}

TEST(Distance,getDistanceExactly5)
{

    Key a("RUXX9tK2i9nhAYn0N2fJ");
    Key b("4FnegdVPduGLY54O5O4R");

    EXPECT_EQ(Distance(a,b).getDistance(), 4);
}

TEST(Distance,getDistanceExactly6)
{

    Key a("6pgabi9UOuSpM2Tlh01v");
    Key b("OzwvSikHFswuzleht6xG");

    EXPECT_EQ(Distance(a,b).getDistance(), 5);
}

TEST(Distance,getDistanceExactly7)
{

    Key a("u0G1JuvMJqw0tOHPAwmL");
    Key b("iIGjRgs2Rp5t0cRLGBXR");

    EXPECT_EQ(Distance(a,b).getDistance(), 6);
}

TEST(Distance,getDistanceExactly8)
{

    Key a("x40P55LKc2l8QBvC0H9M");
    Key b("RDDqnUQ5HKch7RmztXsa");

    EXPECT_EQ(Distance(a,b).getDistance(), 7);
}

TEST(Distance,getDistanceExaclty2MultipleValues)
{
    Key a("AUNYD4iaddbXeeo8vwsH");
    Key b("upNxt1RLw3vMGQThDuAE");

    EXPECT_EQ(Distance(a,b).getDistance(), 2);
}

TEST(Distance,getDistanceSecondByte)
{
    Key a("Ebu87Kn8nfOjhSxpYPaX");
    Key b("QRLZu7w3KUDP9bxgHPic");

    EXPECT_EQ(Distance(a,b).getDistance(),8);
}

TEST(Distance,opLessThan_firstTrue)
{
    Key a("GbhLKrs1BqjbkcKd2fiw");
    Key b("V1HIST1r23It98FtD7Ev");
    Key c("9st9v7VSK5ta3BFcqMqT");
    Key d("pYVhfk81O5mTWrvOGqqL");

    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_secondTrue)
{
   Key a("OWvU6xxcBPp9Ml52CJxw");
   Key b("J5rV89VL9pLBSHecIeyN");
   Key c("WmadfAYyFWFL5CoY5J42");
   Key d("QX659NlqHABReLgSWDwZ");

    EXPECT_TRUE((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_false)
{
    Key a("4pkqRFkElfZqybGpLW4j");
    Key b("lNwY926r33eRhBvggOTz");
    Key c("tz4A1RpsHbP95Ig8jCiR");
    Key d("OCDSg5ukGzwgsSXeCz9m");

    EXPECT_FALSE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opGreaterThan_firstTrue)
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    Key d("OpcBLot6xtl97pGZIPtR");

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_secondTrue)
{
    Key a("XqtsC633kSQB7v0FJ4Rh");
    Key b("r7mZZ8n86HaPZXkjRwjP");
    Key c("gMWf3IWpzH0UA9sqYOLu");
    Key d("e9I6eiDsDmVg85jbg0AA");

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_false)
{
    Key a("Nqowc9eWwpXxBrbePjY8");
    Key b("y82FUInSXBqNrmXEfeu5");
    Key c("ApRYAbv6RgKtLaibOsar");
    Key d("KwkyLDM4mN7BBGs6HRcB");

    EXPECT_FALSE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opLessThanOrEqual)
{
    Key a("GbhLKrs1BqjbkcKd2fiw");
    Key b("V1HIST1r23It98FtD7Ev");
    Key c("9st9v7VSK5ta3BFcqMqT");
    Key d("pYVhfk81O5mTWrvOGqqL");

    EXPECT_TRUE(Distance(a,b) <= Distance(c,d));
    EXPECT_TRUE(Distance(a,b) <= Distance(a,b));
    EXPECT_TRUE(Distance(c,d) <= Distance(c,d));
}

TEST(Distance,opGreaterThanOrEqual)
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    Key d("OpcBLot6xtl97pGZIPtR");

    EXPECT_TRUE(Distance(a,b) >= Distance(c,d));
    EXPECT_TRUE(Distance(a,b) >= Distance(a,b));
    EXPECT_TRUE(Distance(c,d) >= Distance(c,d));
}

TEST(Distance,opEqual)
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    EXPECT_TRUE(Distance(a,b) == Distance(a,b));
    EXPECT_FALSE(Distance(a,b) == Distance(a,c));
}

TEST(Distance,opNotEqual)
{
    Key a("2Tv8cForS9J16ngkrpF9");
    Key b("TqF3AFes5UniXGzhbPiX");
    Key c("NyZO9NhCcluT0ueB5jwC");
    EXPECT_FALSE(Distance(a,b) != Distance(a,b));
    EXPECT_TRUE(Distance(a,b) != Distance(a,c));
}

