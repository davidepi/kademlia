#import <XCTest/XCTest.h>
#include "Messenger.hpp"
#define XCTEST

@interface Messenger_tests : XCTestCase

@end

@implementation Messenger_tests

- (void)setUp {
    [super setUp];
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = new std::queue<Message*>;
    m->init(q, 3400, true);
}

- (void)tearDown {
    [super tearDown];
}

- (void)test00_Messenger_creationEmpty
{
    Messenger::getInstance();
    XCTAssertTrue(true); //no crash = OK ;)
}

- (void)test01_Messenger_initInvalidQueue
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = NULL;
    int retval = m->init(q, 1028, true);
    
    XCTAssertEqual(retval,NULL_QUEUE);
}

- (void)test02_Messenger_doubleInit
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = new std::queue<Message*>();
    m->init(q, 3400,true);
    int retval = m->init(q, 3400, true);
    XCTAssertEqual(retval,ALREADY_INITIALIZED);
}

- (void)test03_Messenger_getIpPublic
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
    XCTAssertTrue(strcmp(ip,ip2)==0);
}

- (void)test04_Messenger_getPort
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q;
    m->init(q,3400,true);
    XCTAssertEqual(m->getPort(),3400);
}

- (void)test05_Message_messageTextConstructor
{
    Message m("ciao");
    XCTAssertEqual(strcmp("ciao", (char*)m.getText()),0);
}

- (void)test06_Message_messageDataConstructor
{
    uint8_t data[] = {0x2A,0xE0,0x88,0x98,0x49,0x0D,0x87,0x5F};
    Message m(data,8);
    
    XCTAssertEqual(data[0],m.getData()[0]);
    XCTAssertEqual(data[1],m.getData()[1]);
    XCTAssertEqual(data[2],m.getData()[2]);
    XCTAssertEqual(data[3],m.getData()[3]);
    XCTAssertEqual(data[4],m.getData()[4]);
    XCTAssertEqual(data[5],m.getData()[5]);
    XCTAssertEqual(data[6],m.getData()[6]);
    XCTAssertEqual(data[7],m.getData()[7]);
}

- (void)test07_Message_privateSenderConstructor
{
    uint8_t data[] = {0x45,0x76};
    Message m("10.4.24.1",55640,2,data,0x1);
    XCTAssertEqual(data[0],m.getText()[0]);
    XCTAssertEqual(data[1],m.getText()[1]);
}

- (void)test08_Message_dataLength
{
    uint8_t data[] = {0x7F,0xB3,0x54,0xCC,0x55,0x4C,0x00,0x36};
    Message m(data,7);
    XCTAssertEqual(m.getLength(), 7);
}

- (void)test09_Message_dataText
{
    char a[] = "For academic purposes only";
    Message m(a);
    XCTAssertEqual(m.getLength(),27);
}

- (void)test10_Message_emptySender
{
    char a[] = "Software modernization";
    Message m(a);
    Node n = m.getSenderNode();
    XCTAssertTrue(n.isEmpty());
}

- (void)test10_Message_craftedSender
{
    uint8_t a[] = "Přihlášení";
    Message m("10.0.0.1",6840,23,a,0x1);
    Node n = m.getSenderNode();
    XCTAssertFalse(n.isEmpty());
    XCTAssertTrue(n.getIp() == "10.0.0.1");
    XCTAssertTrue(n.getPort() == 6840);
}

- (void)test11_Messenger_sendAndReceive
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
        XCTAssertTrue(strcmp(msg.getText(),extracted->getText())==0);
        delete extracted;
    }
    else
        XCTAssertTrue(false);
}

- (void)test12_Messenger_sendAndReceiveCorrectPort
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
        XCTAssertTrue(strcmp(msg.getText(),extracted->getText())==0);
        XCTAssertEqual(extracted->getSenderNode().getIp(),m->getIp());
        XCTAssertEqual(extracted->getSenderNode().getPort(),m->getPort());
        delete extracted;
    }
    else
        XCTAssertTrue(false);
}

- (void)test13_Message_setData
{
    uint8_t data[] = {0x2A,0xE0,0x88,0x98,0x49,0x0D,0x87,0x5F};
    Message m(0x3);
    m.setData(data,8);
    
    XCTAssertEqual(data[0],m.getData()[0]);
    XCTAssertEqual(data[1],m.getData()[1]);
    XCTAssertEqual(data[2],m.getData()[2]);
    XCTAssertEqual(data[3],m.getData()[3]);
    XCTAssertEqual(data[4],m.getData()[4]);
    XCTAssertEqual(data[5],m.getData()[5]);
    XCTAssertEqual(data[6],m.getData()[6]);
    XCTAssertEqual(data[7],m.getData()[7]);
    XCTAssertEqual(m.getFlags(), 0x3);
}

- (void)test14_Message_setText
{
    Message m(0x7);
    m.setText("Kuřecí maso");
    XCTAssertEqual(strcmp("Kuřecí maso", (char*)m.getText()),0);
    XCTAssertEqual(m.getFlags(), 0x7);
}

- (void)test15_Message_append
{
    Message m(0x5);
    uint8_t data1[] = {'H','e','l','l','o'};
    m.setData(data1,5);
    uint8_t data2[] = {' ','W','o','r','l','d','!','\0'};
    m.append(data2, 8);
    const char* text = m.getText();
    
    XCTAssertEqual(strcmp(text,"Hello World!"), 0);
}

@end
