/* 
 * File:   edaGreedyMethod.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 5:14 PM
 */

#ifndef EDAGREEDYSTATEGY_H
#define	EDAGREEDYSTATEGY_H

#include "edaStrategy.h"


class edaGreedyStrategy: public edaStrategy 
{
public:

    edaGreedyStrategy() {}
    
    virtual ~edaGreedyStrategy() {}
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;
    
    virtual edaGreedyStrategy* clone() const = 0;
    
    virtual void checkError() const = 0;    

    virtual const char* className() const = 0;

};


#endif	/* EDAGREEDYSTATEGY_H */

