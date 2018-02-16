#include "Messenger.hpp"
#define XCTEST



#include <gtest/gtest.h>













TEST(Messenger,creationEmpty)
{
    Messenger::getInstance();
    EXPECT_TRUE(true); //no crash = OK ;)
}

TEST(Messenger,initInvalidQueue)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = NULL;
    int retval = m->init(q, 1028, true);

    EXPECT_EQ(retval,NULL_QUEUE);
}

TEST(Messenger,doubleInit)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = new std::queue<Message*>();
    m->init(q, 3400,true);
    int retval = m->init(q, 3400, true);
    EXPECT_EQ(retval,ALREADY_INITIALIZED);
}

TEST(Messenger,getIpPublic)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>*q = new std::queue<Message*>();
    m->init(q,3400,true);

    //-s -N is otherwise curl does not close the buffer correctly
    FILE* fp = popen("curl -s -N http://ipinfo.io/ip | xargs echo -n","r");
    //xargs echo -n becuse otherwise it will print a line break at the end
    char ip[20];
    char ip2[20];
    fgets(ip,sizeof(ip)-1,fp);
    fclose(fp);

    m->getIp().toString(ip2);
    EXPECT_TRUE(strcmp(ip,ip2)==0);
}

TEST(Messenger,getPort)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q;
    m->init(q,3400,true);
    EXPECT_EQ(m->getPort(),3400);
}

TEST(Message,messageTextConstructor)
{
    Message m("ciao");
    EXPECT_EQ(strcmp("ciao", (char*)m.getText()),0);
}

TEST(Message,messageDataConstructor)
{
    uint8_t data[] = {0x2A,0xE0,0x88,0x98,0x49,0x0D,0x87,0x5F};
    Message m(data,8);

    EXPECT_EQ(data[0],m.getData()[0]);
    EXPECT_EQ(data[1],m.getData()[1]);
    EXPECT_EQ(data[2],m.getData()[2]);
    EXPECT_EQ(data[3],m.getData()[3]);
    EXPECT_EQ(data[4],m.getData()[4]);
    EXPECT_EQ(data[5],m.getData()[5]);
    EXPECT_EQ(data[6],m.getData()[6]);
    EXPECT_EQ(data[7],m.getData()[7]);
}

TEST(Message,privateSenderConstructor)
{
    uint8_t data[] = {0x45,0x76};
    Message m("10.4.24.1",55640,2,data,0x1);
    EXPECT_EQ(data[0],m.getText()[0]);
    EXPECT_EQ(data[1],m.getText()[1]);
}

TEST(Message,dataLength)
{
    uint8_t data[] = {0x7F,0xB3,0x54,0xCC,0x55,0x4C,0x00,0x36};
    Message m(data,7);
    EXPECT_EQ(m.getLength(), 7);
}

TEST(Message,dataText)
{
    char a[] = "For academic purposes only";
    Message m(a);
    EXPECT_EQ(m.getLength(),27);
}

TEST(Message,emptySender)
{
    char a[] = "Software modernization";
    Message m(a);
    Node n = m.getSenderNode();
    EXPECT_TRUE(n.isEmpty());
}

TEST(Message,craftedSender)
{
    uint8_t a[] = "Přihlášení";
    Message m("10.0.0.1",6840,23,a,0x1);
    Node n = m.getSenderNode();
    EXPECT_FALSE(n.isEmpty());
    EXPECT_TRUE(n.getIp() == "10.0.0.1");
    EXPECT_TRUE(n.getPort() == 6840);
}

TEST(Messenger,sendAndReceive)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q;
    m->init(q,3400,true);
    Message msg("Přijet pozdě");
    m->sendMessage(Node(m->getIp(), 3400), msg);
    sleep(1);
    if(q->size())
    {
        Message* extracted = q->front();
        q->pop();
        EXPECT_TRUE(strcmp(msg.getText(),extracted->getText())==0);
        delete extracted;
    }
    else
        EXPECT_TRUE(false);
}

TEST(Messenger,sendAndReceiveCorrectPort)
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q;
    m->init(q,3400,true);
    m->setPrivate();
    Message msg("Bořislavka");
    m->sendMessage(Node(m->getIp(), 3400), msg);
    sleep(1);
    if(q->size())
    {
        Message* extracted = q->front();
        q->pop();
        EXPECT_TRUE(strcmp(msg.getText(),extracted->getText())==0);
        EXPECT_EQ(extracted->getSenderNode().getIp(),m->getIp());
        EXPECT_EQ(extracted->getSenderNode().getPort(),m->getPort());
        delete extracted;
    }
    else
        EXPECT_TRUE(false);
}

TEST(Message,setData)
{
    uint8_t data[] = {0x2A,0xE0,0x88,0x98,0x49,0x0D,0x87,0x5F};
    Message m(0x3);
    m.setData(data,8);

    EXPECT_EQ(data[0],m.getData()[0]);
    EXPECT_EQ(data[1],m.getData()[1]);
    EXPECT_EQ(data[2],m.getData()[2]);
    EXPECT_EQ(data[3],m.getData()[3]);
    EXPECT_EQ(data[4],m.getData()[4]);
    EXPECT_EQ(data[5],m.getData()[5]);
    EXPECT_EQ(data[6],m.getData()[6]);
    EXPECT_EQ(data[7],m.getData()[7]);
    EXPECT_EQ(m.getFlags(), 0x3);
}

TEST(Message,setText)
{
    Message m(0x7);
    m.setText("Kuřecí maso");
    EXPECT_EQ(strcmp("Kuřecí maso", (char*)m.getText()),0);
    EXPECT_EQ(m.getFlags(), 0x7);
}

TEST(Message,append)
{
    Message m(0x5);
    uint8_t data1[] = {'H','e','l','l','o'};
    m.setData(data1,5);
    uint8_t data2[] = {' ','W','o','r','l','d','!','\0'};
    m.append(data2, 8);
    const char* text = m.getText();

    EXPECT_EQ(strcmp(text,"Hello World!"), 0);
}

