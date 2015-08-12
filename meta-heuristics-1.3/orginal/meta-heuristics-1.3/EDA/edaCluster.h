/* 
 * File:   edaCluster.h
 * Author: Tieu Minh
 *
 * Created on April 10, 2014, 8:04 PM
 */

#ifndef EDACLUSTER_H
#define	EDACLUSTER_H

#include "edaDAGVertex.h"

class edaCluster: public vector<edaDAGVertex*>  {
public:

    edaCluster() : vector<edaDAGVertex*>()  
    {        
    }

    virtual ~edaCluster() 
    {
        clear();
    }

    bool isOverlap(const edaCluster* clus) 
    {
        return isOverlap(*clus);
    }
    
    bool isOverlap(const edaCluster& clus) 
    {
        unsigned int count = 0;
        for(unsigned int i = 0; i < size(); i++) {
            unsigned int key = at(i)->getKey();
            for(unsigned int j = 0; j < clus.size(); j++) 
            {
                if(key == clus[j]->getKey() ) 
                {
                    count ++;
                    break;
                }
            }
            if(count > 1)
                return true;
        }
        return false;  
    }

    bool isCover(const edaCluster* clus)
    {
        return isCover(*clus);
    }
        
    bool isCover(const edaCluster& clus)
    {
        unsigned int i, j;
        for(i = 0; i < clus.size(); i++) 
        {
            unsigned int key = clus[i]->getKey();
            for(j = 0; j < size(); j++) 
            {
                if(key == at(j)->getKey())
                    break;
            }
            if(j == size() )
                return false;
        }
        return true; 
    }

    void printOn(ostream &os) const 
    {
        for(unsigned int i = 0; i < size() - 1 ; i++) 
        {
            os << at(i)->getKey() << " ";
        }
        os << at( size() - 1 )->getKey();
    }

    virtual const char* className() const 
    {
        return "edaCluster";
    }
};
inline std::ostream& operator<< (std::ostream &os, const edaCluster &clus) 
{
    clus.printOn(os);
    return os;
}

#endif	/* EDACLUSTER_H */

