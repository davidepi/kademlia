#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
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

	private:
		pthread_t thread_id;
        Kbucket kBucketArray[NBYTE*8];
};


#endif