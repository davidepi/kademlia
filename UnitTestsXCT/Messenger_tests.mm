#import <XCTest/XCTest.h>
#include "../src/Messenger.hpp"
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
    int retval = m->init(q, 3400);
    delete q;
    XCTAssertEqual(retval,ALREADY_INITIALIZED);
}

- (void)test03_Messenger_getIp
{
    Messenger* m = &(Messenger::getInstance());
    std::queue<Message*>*q = new std::queue<Message*>();
    m->init(q,3400);
    
    
    
    //If fails: check the interface and change en0 to a proper one
    FILE* fp = popen("/usr/sbin/ipconfig getifaddr en0 | xargs echo -n","r");
    //xargs echo -n becuse otherwise it will print a line break at the end
    //and strcmp will fail
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
    Message r(RPC_FIND_NODE);
    Message t(RPC_FIND_VALUE);
    Message y(RPC_PING | RPC_PONG);
    Message a(RPC_PING | RPC_STORE);
    Message s(RPC_PING | RPC_FIND_NODE);
    Message f(RPC_PING | RPC_FIND_VALUE);
    Message g(RPC_PONG | RPC_STORE);
    Message z(RPC_PONG | RPC_FIND_NODE);
    Message x(RPC_PONG | RPC_FIND_VALUE);
    Message c(RPC_STORE | RPC_FIND_NODE);
    Message v(RPC_STORE | RPC_FIND_VALUE);
    Message b(RPC_FIND_NODE | RPC_FIND_VALUE);
    
    //combinazioni da 3 sono inutili, gia' alcune di queste lo sono
    //tipo la ping e la pong assieme. Mi basta sapere che la getFlags funziona
    //anche per le combinazioni multiple
    
    XCTAssertEqual(RPC_PING, q.getFlags());
    XCTAssertEqual(RPC_PONG, w.getFlags());
    XCTAssertEqual(RPC_STORE, e.getFlags());
    XCTAssertEqual(RPC_FIND_NODE, r.getFlags());
    XCTAssertEqual(RPC_PING | RPC_PONG, y.getFlags());
    XCTAssertEqual(RPC_PING | RPC_STORE, a.getFlags());
    XCTAssertEqual(RPC_PING | RPC_FIND_NODE, s.getFlags());
    XCTAssertEqual(RPC_FIND_VALUE, t.getFlags());
    XCTAssertEqual(RPC_PING | RPC_FIND_VALUE, f.getFlags());
    XCTAssertEqual(RPC_PONG | RPC_STORE, g.getFlags());
    XCTAssertEqual(RPC_PONG | RPC_FIND_NODE, z.getFlags());
    XCTAssertEqual(RPC_PONG | RPC_FIND_VALUE, x.getFlags());
    XCTAssertEqual(RPC_STORE | RPC_FIND_NODE, c.getFlags());
    XCTAssertEqual(RPC_STORE | RPC_FIND_VALUE, v.getFlags());
    XCTAssertEqual(RPC_FIND_NODE | RPC_FIND_VALUE, b.getFlags());
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
    std::queue<Message*>* q = m->getBindedQueue();
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
    std::queue<Message*>* q = m->getBindedQueue();
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

@end
