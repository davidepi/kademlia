#import <XCTest/XCTest.h>
#include "Kbucket.hpp"
#include "Messenger.hpp"
#define XCTEST

@interface Kbucket_tests : XCTestCase

@end

@implementation Kbucket_tests

- (void)test00_Kbucket_creationEmpty
{
    Kbucket k;
    XCTAssertTrue(true);
}

- (void)test01_Kbucket_deletion
{
    Kbucket* k = new Kbucket();
    delete k;
}

- (void)test02_Kbucker_getSize
{
    Kbucket k;
    XCTAssertEqual(k.getSize(), 0);
    
    k.addNode(Node("127.0.0.1",3400));
    XCTAssertEqual(k.getSize(), 1);
}

- (void)test03_Kbucket_addNode
{
    //empty space and node not present
    Kbucket k;
    Node me("127.0.0.1",3400);
    k.addNode(me);
    for(int i=1;i<KBUCKET_SIZE;i++)
    {
        Node n("127.0.0.1",3400+i);
        k.addNode(n);
    }
    XCTAssertEqual(k.getSize(),KBUCKET_SIZE);
    
    //add myself
    std::list<Node>* list = k.getNodes();
    XCTAssertEqual(list->back().getPort(), 3400); //I am the last one
    XCTAssertNotEqual(list->front().getPort(), 3400); //I am not the first one
    k.addNode(me);
    XCTAssertEqual(list->front().getPort(), 3400); //I am the first one
    XCTAssertNotEqual(list->back().getPort(), 3400); //I am not the last one
    
    //full space and node not present
    std::queue<Message*>* q;    //without Messenger the socket will throw exc.
    Messenger::getInstance().init(q,3400);
    Node extra("127.0.0.1",10000);
    k.addNode(extra);
    XCTAssertTrue(true);//no need to assert presence, it will be a duty of the
                        //updater class
}

- (void)test04_Kbucket_getNodes
{
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    
    std::list<Node>* list = k.getNodes();
    int i=KBUCKET_SIZE-1; //used to assert the port, in reverse order
    for(std::list<Node>::iterator j=list->begin();j!=list->end();++j)
    {
        XCTAssertEqual(j->getIp(), def);
        XCTAssertEqual(j->getPort(), 3400+i);
        i--;
    }
}

- (void)test05_Kbucket_setNodes
{
    //add some nodes
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    
    //change node vals
    std::list<Node>* list = k.getNodes();
    int i = 0;
    for(std::list<Node>::iterator j=list->begin();j!=list->end();++j)
    {
        *j = Node(def,6400+i);
        i++;
    }
    
    //set vals
    k.setNodes(list);
    
    //retrieve nodes and assert them
    list = k.getNodes();
    i=0; //used to assert the port, in reverse order
    for(std::list<Node>::iterator j=list->begin();j!=list->end();++j)
    {
        XCTAssertEqual(j->getIp(), def);
        XCTAssertEqual(j->getPort(), 6400+i);
        i++;
    }
}

- (void)test06_Kbucket_serialize_deserialize
{
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    
    uint8_t data[500];
    k.serialize(data);
    
    //just check the size
    uint16_t node_number = ntohs(((uint16_t*)data)[0]);
    XCTAssertEqual(node_number, KBUCKET_SIZE);
    
    //craft another bucket
    Kbucket k2(data);
    
    //ensure the two buckets are identical
    std::list<Node>* list2 = k2.getNodes();
    //need to transform comparison list into a vector for O(1) access

    int i=0;
    std::vector<Node> compare;
    compare.insert(compare.end(),k.getNodes()->begin(), k.getNodes()->end());
    for(std::list<Node>::iterator j=list2->begin();j!=list2->end();++j)
    {
        XCTAssertEqual(j->getIp(),compare.at(i).getIp());
        XCTAssertEqual(j->getPort(),compare.at(i).getPort());
        i++;
    }
}

- (void)test07_Kbucket_contains
{
#if KBUCKET_SIZE < 5
#warning "Test will search the 5th position in the Kbucket and fail"
#endif
    
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    
    //check existing
    Node yes("127.0.0.1",3404);
    XCTAssertTrue(k.contains(&yes));
    
    //check non-existing
    Node no("127.0.0.1",3800);
    XCTAssertFalse(k.contains(&no));
}

- (void)test08_Kbucket_print
{
    //can't test the print, just trying the run
    Kbucket k;
    Ip def("127.0.0.1");
    for(int i=0;i<KBUCKET_SIZE;i++)
    {
        Node n(def,3400+i);
        k.addNode(n);
    }
    k.print();
}

- (void)test09_Kbucket_delete
{
    //empty space and node not present
    Kbucket k;
    Node me("127.0.0.1",3400);
    k.addNode(me);
    for(int i=1;i<KBUCKET_SIZE;i++)
    {
        Node n("127.0.0.1",3400+i);
        k.addNode(n);
    }
    XCTAssertEqual(k.getSize(),KBUCKET_SIZE);
    k.deleteNode(Node("127.0.0.1",3404));
    
    XCTAssertEqual(k.getSize(), KBUCKET_SIZE-1);
}

@end
