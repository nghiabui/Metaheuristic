/* 
 * File:   edaNeighbourSelect.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 8:21 PM
 */

#ifndef EDASTATEGY_H
#define	EDASTATEGY_H

#include "edaPopulation.h"
#include "edaNeighbour.h"


class edaStrategy: public edaSerialize {
public:

    edaStrategy() {}
    
    virtual ~edaStrategy() {}
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;
    
    virtual edaStrategy* clone() const = 0;

    virtual void checkError() const = 0;    

    virtual const char* className() const = 0;

    
};

#endif	/* EDASTATEGY_H */

