#ifndef __PERFORMER_HPP__
#define __PERFORMER_HPP__

#include <queue>
#include <unistd.h>
#include "Messenger.hpp"

class Performer{

	public: 
		static Performer& getInstance();
		void init(std::queue<Message*>* q);

	private:
		Performer();
		pthread_t thread_id;
};


#endif