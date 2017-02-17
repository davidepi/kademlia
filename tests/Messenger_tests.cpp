#include "../src/Messenger.hpp"
#define XCTEST



#include <gtest/gtest.h>







TEST(Messenger,creationEmpty)
{
    Messenger* m = &(Messenger::getInstance());

    EXPECT_TRUE(true); //no crash = OK ;)
}

TEST(Messenger,initInvalidQueue)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = NULL;
    int retval = m->init(q, 1028);

    EXPECT_EQ(retval,NULL_QUEUE);
}

TEST(Messenger,doubleInit)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = new std::queue<Message*>();
    std::queue<Message*>* r = new std::queue<Message*>();
    m->init(q, 3400);
    int retval = m->init(r, 3500);

    EXPECT_EQ(retval,ALREADY_INITIALIZED);
}

TEST(Messenger,getIp)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>*q = new std::queue<Message*>();
    m->init(q,3400);
    m->getIp();
    EXPECT_TRUE(false);
}

TEST(Messenger,getPort)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>*q = new std::queue<Message*>();
    m->init(q,3400);
    m->getPort();
    EXPECT_TRUE(false);
}

TEST(Message,messageTextConstructor)
{
    Message m("ciao",RPC_FIND_NODE);
    EXPECT_EQ(strcmp("ciao", m.getText()),0);
    EXPECT_EQ(RPC_FIND_NODE, m.getFlags());
}

TEST(Message,messageDataConstructor)
{
    uint8_t data[] = {0x2A,0xE0,0x88,0x98,0x49,0x0D,0x87};
    Message m(data,RPC_PING);
    EXPECT_EQ(strcmp((char*)data, m.getText()),0);
    EXPECT_EQ(RPC_PING, m.getFlags());
}

TEST(Message,messageAllFlagsCombinations)
{
    Message q("ciao",RPC_PING);
    Message w("ciao",RPC_PONG);
    Message e("ciao",RPC_STORE);
    Message r("ciao",RPC_FIND_NODE);
    Message t("ciao",RPC_FIND_VALUE);
    Message y("ciao",RPC_PING | RPC_PONG);
    Message a("ciao",RPC_PING | RPC_STORE);
    Message s("ciao",RPC_PING | RPC_FIND_NODE);
    Message f("ciao",RPC_PING | RPC_FIND_VALUE);
    Message g("ciao",RPC_PONG | RPC_STORE);
    Message z("ciao",RPC_PONG | RPC_FIND_NODE);
    Message x("ciao",RPC_PONG | RPC_FIND_VALUE);
    Message c("ciao",RPC_STORE | RPC_FIND_NODE);
    Message v("ciao",RPC_STORE | RPC_FIND_VALUE);
    Message b("ciao",RPC_FIND_NODE | RPC_FIND_VALUE);

    //combinazioni da 3 sono inutili, gia' alcune di queste lo sono
    //tipo la ping e la pong assieme. Mi basta sapere che la getFlags funziona
    //anche per le combinazioni multiple

    EXPECT_EQ(RPC_PING, q.getFlags());
    EXPECT_EQ(RPC_PONG, w.getFlags());
    EXPECT_EQ(RPC_STORE, e.getFlags());
    EXPECT_EQ(RPC_FIND_NODE, r.getFlags());
    EXPECT_EQ(RPC_PING | RPC_PONG, y.getFlags());
    EXPECT_EQ(RPC_PING | RPC_STORE, a.getFlags());
    EXPECT_EQ(RPC_PING | RPC_FIND_NODE, s.getFlags());
    EXPECT_EQ(RPC_FIND_VALUE, t.getFlags());
    EXPECT_EQ(RPC_PING | RPC_FIND_VALUE, f.getFlags());
    EXPECT_EQ(RPC_PONG | RPC_STORE, g.getFlags());
    EXPECT_EQ(RPC_PONG | RPC_FIND_NODE, z.getFlags());
    EXPECT_EQ(RPC_PONG | RPC_FIND_VALUE, x.getFlags());
    EXPECT_EQ(RPC_STORE | RPC_FIND_NODE, c.getFlags());
    EXPECT_EQ(RPC_STORE | RPC_FIND_VALUE, v.getFlags());
    EXPECT_EQ(RPC_FIND_NODE | RPC_FIND_VALUE, b.getFlags());
}

