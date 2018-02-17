#include "Distance.hpp"
#define XCTEST


#include <gtest/gtest.h>












#ifdef LEFT_DISTANCE

TEST(Distance,SameKey)
{
    Key a("key1"); //0xb299ff9e....
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), 0);
}

TEST(Distance,SameNode)
{
    Node a("127.0.0.1",3400);
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), 0);
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
    Key a("mI2M3H9DF0jEUJfQd7KF");
    Key b("iNkv6lJrS7q58TLLiXrX");

    EXPECT_EQ(Distance(a,b).getDistance(), 159);
}

TEST(Distance,getDistanceExactly2)
{
    Key a("uO4bYNqsErHBrSJFi7A6");
    Key b("yqwmXZ2pm3vp5Q6QSvRA");

    EXPECT_EQ(Distance(a,b).getDistance(), 158);
}

TEST(Distance,getDistanceExactly3)
{
    Key a("cQfXus9rVDn69jrBjxAZ");
    Key b("8oqz8WjRLimCbxbwMGH1");

    EXPECT_EQ(Distance(a,b).getDistance(), 157);
}

TEST(Distance,getDistanceExactly4)
{
    Key a("r3acO9eUk8QLIfyXISJS");
    Key b("IwalOiKKIvhi5XromFnQ");

    EXPECT_EQ(Distance(a,b).getDistance(), 156);
}

TEST(Distance,getDistanceExactly5)
{
    Key a("Wr6OhEyxqTp5BfTmT8GE");
    Key b("Zuw8coZqDZVOaAoLA7da");

    EXPECT_EQ(Distance(a,b).getDistance(), 155);
}

TEST(Distance,getDistanceExactly6)
{
    Key a("gubM7o3H1QIsS2KzUMdq");
    Key b("vsgNMK41xhYBvGBY22mY");

    EXPECT_EQ(Distance(a,b).getDistance(), 154);
}

TEST(Distance,getDistanceExactly7)
{
    Key a("TrIjaCvYcR9fmIpdJtiY");
    Key b("KQKKEbS8La43QzihhHfp");

    EXPECT_EQ(Distance(a,b).getDistance(), 153);
}

TEST(Distance,getDistanceExactly8)
{
    Key a("aXqAOvmRqYqtWzyP4Ql9");
    Key b("bDUCvHoQRtRQPaBRTezZ");

    EXPECT_EQ(Distance(a,b).getDistance(), 152);
}

TEST(Distance,getDistanceExaclty9)
{
    Key a("fFVbcLONJkrrCFqkSji8");
    Key b("5DwKQIWPTboPPNtLCIQp");

    EXPECT_EQ(Distance(a,b).getDistance(), 151);
}

TEST(Distance,getDistanceExaclty10)
{
    Key a("JraYURzLLPseQIkJeC3j");
    Key b("2iw1ftROyBSEXPFk5GE6");

    EXPECT_EQ(Distance(a,b).getDistance(), 150);
}

TEST(Distance,opLessThan_firstTrue)
{
    Key a("B9I9bowhr8Uwjz1EtCcs");
    Key b("FE1Ux1JklAW6XyKUjvTd");
    Key c("415Qfrbp3wwWlWpcVED3");
    Key d("3YpnWdI6y2s4xOlu87Fd");

    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_secondTrue)
{
    Key a("2Of1JaAZcLDpLsjfa7FU");
    Key b("1ml9OuX3vnVD7xJm98hu");
    Key c("cOprmqAtINB3LH8A0s2Q");
    Key d("mar4xEmDCoULPALP4I1G");

    EXPECT_TRUE((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_false)
{
    Key a("I4BDqWrMeOn5lNDSnJ6b");
    Key b("to7NIrVjojuY9myOl4kq");
    Key c("wSVefGQYXZVOGCsKYxH8");
    Key d("UZs2KJH7vktZRyhJzX8y");

    EXPECT_FALSE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opGreaterThan_firstTrue)
{
    Key a("rY0jueMRn7SrFyRRNXEj");
    Key b("S9PRXlXDO6ydeQodqAkt");
    Key c("K2wyacuSW5ohutZi5ocO");
    Key d("97GWjNV3iocIpD9d6Nn3");

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_secondTrue)
{
    Key a("260ZZsnylj95O6qqHkOD");
    Key b("h40ry3q9eafh2g0ncUfR");
    Key c("P5I3OUo24gVjKmYyjgz4");
    Key d("3z7GcywJoFKMXdnw1SLE");

    EXPECT_TRUE((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_false)
{
    Key a("CMbE5zSt1eTnc5Rqtp06");
    Key b("CfsgWZTA0ibkirX4UDca");
    Key c("G9bTwoYVbasotBcu4z1W");
    Key d("pO8f0Txmotr2OnTO0h87");

    EXPECT_FALSE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opLessThanOrEqual)
{
    Key a("9st9v7VSK5ta3BFcqMqT");
    Key b("pYVhfk81O5mTWrvOGqqL");
    Key c("GbhLKrs1BqjbkcKd2fiw");
    Key d("V1HIST1r23It98FtD7Ev");

    EXPECT_TRUE(Distance(a,b) <= Distance(c,d));
    EXPECT_TRUE(Distance(a,b) <= Distance(a,b));
    EXPECT_TRUE(Distance(c,d) <= Distance(c,d));
}

TEST(Distance,opGreaterThanOrEqual)
{
    Key a("NyZO9NhCcluT0ueB5jwC");
    Key b("OpcBLot6xtl97pGZIPtR");
    Key c("2Tv8cForS9J16ngkrpF9");
    Key d("TqF3AFes5UniXGzhbPiX");

    EXPECT_TRUE(Distance(a,b) >= Distance(c,d));
    EXPECT_TRUE(Distance(a,b) >= Distance(a,b));
    EXPECT_TRUE(Distance(c,d) >= Distance(c,d));
}

TEST(Distance,opEqual)
{
    Key a("2Xvj7hEv3yQsYTZsdseP");
    Key b("fjTtJbNqV6yJdzoIjXpK");
    Key c("41YBcDHd57gsaeK7Dw8Y");
    Distance ab(a,b);
    Distance ac(a,c);
    EXPECT_TRUE(Distance(a,b) == Distance(a,b));
    EXPECT_TRUE(Distance(a,b) >= Distance(a,b));
    EXPECT_TRUE(Distance(a,b) <= Distance(a,b));
    EXPECT_FALSE(Distance(a,b) == Distance(a,c));
}

TEST(Distance,opNotEqual)
{
    Key a("4heh1f9WepueKsGYJwh6");
    Key b("8dtFkPTxBBzBznFldSel");
    Key c("pHYGh0lVGL7LwdcM4n0G");
    EXPECT_FALSE(Distance(a,b) != Distance(a,b));
    EXPECT_TRUE(Distance(a,b) != Distance(a,c));
}

#else

TEST(Distance,SameKey)
{
    Key a("key1"); //0xb299ff9e....
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), 0);
}

TEST(Distance,SameNode)
{
    Node a("127.0.0.1",3400);
    Distance d1(a,a);

    EXPECT_EQ(d1.getDistance(), 0);
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
    Key a("U410TcgpXHR3MX35uauQ");
    Key b("SKPC4WnRg9qIG4Cs8eTB");

    EXPECT_EQ(Distance(a,b).getDistance(), 159);
}

TEST(Distance,getDistanceExactly2)
{
    Key a("GiyrJRPkPPL2U3usrs5N");
    Key b("LPoXxRCZBW9OE0IR5IWd");

    EXPECT_EQ(Distance(a,b).getDistance(), 158);
}

TEST(Distance,getDistanceExactly3)
{
    Key a("PatFuSrerMH3FYfqWd1l");
    Key b("p3TmmSNCx9RsQdp6RtpI");

    EXPECT_EQ(Distance(a,b).getDistance(), 157);
}

TEST(Distance,getDistanceExactly4)
{
    Key a("Y6cittEFZAGpaQ1weSCo");
    Key b("HJGNI7GTcMRZPTCg9CKq");

    EXPECT_EQ(Distance(a,b).getDistance(), 156);
}

TEST(Distance,getDistanceExactly5)
{
    Key a("ML184HEWzGznrJt0QbOG");
    Key b("6xVnrfi78nFfmymV4Hoj");

    EXPECT_EQ(Distance(a,b).getDistance(), 155);
}

TEST(Distance,getDistanceExactly6)
{
    Key a("ESzOUutICulVO43UGBE0");
    Key b("JNPJP3AlZ4GXHYUxHYuY");

    EXPECT_EQ(Distance(a,b).getDistance(), 154);
}

TEST(Distance,getDistanceExactly7)
{
    Key a("61l9weepUyj8LMu5W10N");
    Key b("Cvslu0SGkTqAnJuMK7mI");

    EXPECT_EQ(Distance(a,b).getDistance(), 153);
}

TEST(Distance,getDistanceExactly8)
{
    Key a("XaSbYf5eSVz7A3Wke92P");
    Key b("eyZOy2ckDTtPLjfbTPQu");

    EXPECT_EQ(Distance(a,b).getDistance(), 152);
}

TEST(Distance,getDistanceExaclty9)
{
    Key a("7ehlgSK3olahZ8rBwh7t");
    Key b("FbNJbZ4xcmrAURd1t5gm");

    EXPECT_EQ(Distance(a,b).getDistance(), 151);
}

TEST(Distance,getDistanceExaclty10)
{
    Key a("EQ8FllD9FJrLUS27MQx4");
    Key b("bT868cq0FPsdW3L9Coiq");

    EXPECT_EQ(Distance(a,b).getDistance(), 150);
}

TEST(Distance,opLessThan_firstTrue)
{
    Key a("mC7uEWRR6dPa804dyomJ");
    Key b("z2nkOTWUCQ1t8QlNPxnV");
    Key c("umJg3Gfa1ZRWeYlzVZhh");
    Key d("3jChT4hf7lXTcyUod7l1");

    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_secondTrue)
{
    Key a("52SvXqXamfXPFwwdbWuU");
    Key b("XEZcLhLUewcyW3ipCaku");
    Key c("kGaphWqXPv8Y9grvg0cp");
    Key d("ee2LqSWV9UwR8Kpf3Tjr");

    EXPECT_TRUE((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);
    EXPECT_TRUE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opLessThan_false)
{
    Key a("yPAXjRc0cFsw8Mh5ScDh");
    Key b("qlXlw6ZbC8uTGKlygIn3");
    Key c("7N3E6gK7k9rwo8KjDTiH");
    Key d("KyejOtzs2MSs59C1gPa3");

    EXPECT_FALSE(Distance(a,b) < Distance(c,d));
}

TEST(Distance,opGreaterThan_firstTrue)
{
    Key a("IAo80elpLRXxyhDnxNOD");
    Key b("GVMgdkGJeO6DmNjsjdix");
    Key c("Z8MdjsMXIQ9hQnnBmIuH");
    Key d("sdqcXykRIs3wpvvUDlow");

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_secondTrue)
{
    Key a("nyWl6tqqkKse7hZnHv6d");
    Key b("PgwgRKX4umVqQgNa8BPD");
    Key c("EkZWKXKrbfppOYCWEA8V");
    Key d("Ls89FC0R9F2Kq4zS3pIP");

    EXPECT_TRUE((a.getKey()[0] ^ b.getKey()[0]) == 0 &&
                  (c.getKey()[0] ^ d.getKey()[0]) == 0);

    EXPECT_TRUE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opGreaterThan_false)
{
    Key a("h1q4BTZIUx5GtiIqCO4N");
    Key b("lh9DqOMdwGC9iDtZfcid");
    Key c("cchGgueEnfOlafARfQHJ");
    Key d("XsipMqn9r33rzC7csCcO");

    EXPECT_FALSE(Distance(a,b) > Distance(c,d));
}

TEST(Distance,opLessThanOrEqual)
{
    Key a("9st9v7VSK5ta3BFcqMqT");
    Key b("pYVhfk81O5mTWrvOGqqL");
    Key c("GbhLKrs1BqjbkcKd2fiw");
    Key d("V1HIST1r23It98FtD7Ev");

    EXPECT_TRUE(Distance(a,b) <= Distance(c,d));
    EXPECT_TRUE(Distance(a,b) <= Distance(a,b));
    EXPECT_TRUE(Distance(c,d) <= Distance(c,d));
}

TEST(Distance,opGreaterThanOrEqual)
{
    Key a("NyZO9NhCcluT0ueB5jwC");
    Key b("OpcBLot6xtl97pGZIPtR");
    Key c("2Tv8cForS9J16ngkrpF9");
    Key d("TqF3AFes5UniXGzhbPiX");

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
    EXPECT_TRUE(Distance(a,b) >= Distance(a,b));
    EXPECT_TRUE(Distance(a,b) <= Distance(a,b));
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

#endif
