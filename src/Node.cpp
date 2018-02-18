#include "Node.hpp"
#include "Distance.hpp"

//pool of keys and references, used to avoid a lot of `new` and `delete` calls
class KeyDispatcher
{
public:
    KeyDispatcher();
    ~KeyDispatcher();
    Key* acquireKey();
    void freeKey(Key*);
    uint8_t* acquireRef();
    void freeRef(uint8_t* ref);
private:
    Key** keys;
    unsigned int keys_index;
    uint8_t** refs;
    unsigned int refs_index;
    Spinlock spinlock;
};

//another global var, but it is limited to this compile unit
static KeyDispatcher dispatcher;

std::ostream& operator<<(std::ostream &strm, const Node& n)
{ 
    char ip[16];
    n.getIp().toString(ip);
    return strm << ip << ":" << n.getPort();
}

Node::Node()
{
    Node::reference = dispatcher.acquireRef();
    *(Node::reference) = 1;
    Node::my_ip = Ip();
    Node::port_ho = 0;
    Node::id = dispatcher.acquireKey();
    *Node::id = Key((uint32_t)_LOCALHOST_NO_,0);
}

Node::Node(Ip ip, uint16_t port)
{
    Node::reference = dispatcher.acquireRef();
    *(Node::reference) = 1;
	Node::my_ip = ip;
	Node::port_ho = port;
    Node::id = dispatcher.acquireKey();
    *Node::id = Key(ip,htons(port));
}

Node::Node(const Node& copied)
{
    Node::reference = copied.reference;
    (*(Node::reference))++;
    Node::my_ip = copied.my_ip;
    Node::port_ho = copied.port_ho;
    Node::id = copied.id;
}

Node& Node::operator=(const Node& copied)
{
    Node::reference = copied.reference;
    (*(Node::reference))++;
    Node::my_ip = copied.my_ip;
    Node::port_ho = copied.port_ho;
    Node::id = copied.id;
    return *this;
}

bool Node::operator==(const Node &n)const
{
    return Node::my_ip == n.my_ip && Node::port_ho == n.port_ho;
}

bool Node::operator!=(const Node&n)const
{
    return !(*(this)==n);
}

bool Node::operator<(const Node &n)const
{
    uint64_t thisval = Node::my_ip.getIp();
    uint64_t otherval = n.getIp().getIp();
    thisval<<=32;
    otherval<<=32;
    thisval|=Node::port_ho;
    otherval|=n.getPort();
    return thisval<otherval;
}

Ip Node::getIp() const
{
	return my_ip;
}

uint16_t Node::getPort() const
{
	return port_ho;
}

const Key* Node::getKey() const
{
	return id;
}

bool Node::isEmpty()
{
    if(port_ho==0 || my_ip.getIp()==0)
        return true;
    else
        return false;
}

Node::~Node()
{
    if(--(*(Node::reference)) == 0)
    {
        dispatcher.freeKey(Node::id);
        dispatcher.freeRef(Node::reference);
    }
}

KeyDispatcher::KeyDispatcher()
{
    KeyDispatcher::keys = new Key*[NODES_LENGTH];
    KeyDispatcher::refs = new uint8_t*[NODES_LENGTH];
    for(int i=0;i<NODES_LENGTH;i++)
    {
        KeyDispatcher::keys[i] = new Key();
        KeyDispatcher::refs[i] = new uint8_t[1];
    }
    KeyDispatcher::keys_index = 0;
    KeyDispatcher::refs_index = 0;
}

KeyDispatcher::~KeyDispatcher()
{
    for(int i=0;i<QUEUE_LENGTH;i++)
    {
        delete KeyDispatcher::keys[i];
        delete KeyDispatcher::refs[i];
    }
    delete KeyDispatcher::keys;
    delete KeyDispatcher::refs;
}

Key* KeyDispatcher::acquireKey()
{
    Key* retval;
    spinlock.lock();
    retval = KeyDispatcher::keys[KeyDispatcher::keys_index++];
    spinlock.unlock();
    return retval;
}

void KeyDispatcher::freeKey(Key* val)
{
    spinlock.lock();
    KeyDispatcher::keys[--KeyDispatcher::keys_index] = val;
    spinlock.unlock();
}

uint8_t* KeyDispatcher::acquireRef()
{
    uint8_t* retval;
    spinlock.lock();
    retval = KeyDispatcher::refs[KeyDispatcher::refs_index++];
    spinlock.unlock();
    return retval;
}

void KeyDispatcher::freeRef(uint8_t* val)
{
    spinlock.lock();
    KeyDispatcher::refs[--KeyDispatcher::refs_index] = val;
    spinlock.unlock();
}
