/* 
 * File:   edaAspirCrit.h
 * Author: Tieu Minh
 *
 * Created on April 7, 2014, 12:10 AM
 */

#ifndef EDAASPIRCRIT_H
#define	EDAASPIRCRIT_H

#include "edaSerialize.h"
#include "edaStrategy.h"


class edaAspirCrit : public edaStrategy 
{
public:
    
    edaAspirCrit() {}
    
    virtual ~edaAspirCrit() {}
     
    virtual void set( const edaGreedyStrategy& stgy ) = 0;
    
    virtual edaAspirCrit* clone() const = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) = 0;
    
    virtual const char* className() const = 0;
    
    virtual void init() = 0;
    
    virtual bool check(edaNeighbour& neighb) = 0;
    
};

#endif	/* EDAASPIRCRIT_H */

