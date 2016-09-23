#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include <unordered_map>
#include "Messenger.hpp"
#include "Node.hpp"
#include "Kbucket.hpp"
#include "Distance.hpp"

class Performer
{

	public:
		Performer(std::queue<Message*>* q);
        ~Performer();
        std::queue<Message*>* message_queue;
        std::unordered_map<std::string, char*> filesMap;

	private:
		pthread_t thread_id;
        Kbucket kBucketArray[NBYTE*8];
};


#endif