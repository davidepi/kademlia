#ifndef __NEIGHBOURMANAGER_HPP
#define	__NEIGHBOURMANAGER_HPP

#include "Kbucket.hpp"

class NeighbourManager {
    public:
        NeighbourManager();
        ~NeighbourManager();
        const Kbucket* findKClosestNodes(const Key* key);
        const Kbucket* getNeighbourManager();

    private:
        Kbucket neighboursArray[NBYTE * 8];

};

#endif

