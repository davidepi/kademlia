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
    m->init(q, 3400);
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
    int retval = m->init(q, 1028);
    
    XCTAssertEqual(retval,NULL_QUEUE);
}

- (void)test02_Messenger_doubleInit
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q = new std::queue<Message*>();
    m->init(q, 3400);
    int retval = m->init(q, 3400);
    XCTAssertEqual(retval,ALREADY_INITIALIZED);
}

- (void)test03_Messenger_getIpPublic
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>*q = new std::queue<Message*>();
    m->init(q,3400);
    
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
    m->init(q,3400);
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
    Message m("10.4.24.1",55640,2,data,RPC_STORE);
    XCTAssertEqual(data[0],m.getText()[0]);
    XCTAssertEqual(data[1],m.getText()[1]);
}

- (void)test07_Message_messageAllFlagsCombinations
{
    Message q(RPC_PING);
    Message w(RPC_PONG);
    Message e(RPC_STORE);
    Message r(RPC_FIND_NODE_REQUEST);
    Message t(RPC_FIND_NODE_ANSWER);
    Message y(RPC_FIND_NODE_RESPONSE);
    Message u(RPC_FIND_NODE_REQUEST  | FIND_VALUE_FLAG);
    Message i(RPC_FIND_NODE_ANSWER   | FIND_VALUE_FLAG);
    Message o(RPC_FIND_NODE_RESPONSE | FIND_VALUE_FLAG);
    
    //combinazioni da 3 sono inutili, gia' alcune di queste lo sono
    //tipo la ping e la pong assieme. Mi basta sapere che la getFlags funziona
    //anche per le combinazioni multiple
    
    XCTAssertEqual(RPC_PING, q.getFlags());
    XCTAssertEqual(RPC_PONG, w.getFlags());
    XCTAssertEqual(RPC_STORE, e.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_REQUEST, r.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_ANSWER, t.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_RESPONSE, y.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_REQUEST| FIND_VALUE_FLAG, u.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_ANSWER| FIND_VALUE_FLAG, i.getFlags());
    XCTAssertEqual(RPC_FIND_NODE_RESPONSE| FIND_VALUE_FLAG, o.getFlags());
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
    Message m("10.0.0.1",6840,23,a,RPC_STORE);
    Node n = m.getSenderNode();
    XCTAssertFalse(n.isEmpty());
    XCTAssertTrue(n.getIp() == "10.0.0.1");
    XCTAssertTrue(n.getPort() == 6840);
}

- (void)test11_Messenger_sendAndReceive
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>* q;
    m->init(q,3400);
    q = m->getBindedQueue();
    m->setPrivate();
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
    m->init(q,3400);
    m->setPrivate();
    q = m->getBindedQueue();
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
    Message m(RPC_PONG);
    m.setData(data,8);
    
    XCTAssertEqual(data[0],m.getData()[0]);
    XCTAssertEqual(data[1],m.getData()[1]);
    XCTAssertEqual(data[2],m.getData()[2]);
    XCTAssertEqual(data[3],m.getData()[3]);
    XCTAssertEqual(data[4],m.getData()[4]);
    XCTAssertEqual(data[5],m.getData()[5]);
    XCTAssertEqual(data[6],m.getData()[6]);
    XCTAssertEqual(data[7],m.getData()[7]);
    XCTAssertEqual(m.getFlags(), RPC_PONG);
}

- (void)test14_Message_setText
{
    Message m(RPC_FIND_NODE_REQUEST);
    m.setText("Kuřecí maso");
    XCTAssertEqual(strcmp("Kuřecí maso", (char*)m.getText()),0);
    XCTAssertEqual(m.getFlags(), RPC_FIND_NODE_REQUEST);
}

@end
