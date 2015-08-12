/* 
 * File:   edaTabuList.h
 * Author: Tieu Minh
 *
 * Created on April 21, 2014, 9:37 AM
 */

#ifndef EDATABULIST_H
#define	EDATABULIST_H

#include "edaSerialize.h"
#include "edaNeighbour.h"


class edaTabuList : public edaSerialize
{
public:
    
    edaTabuList() {}
    
    virtual ~edaTabuList() {}
    
    virtual void init() = 0;
    
    virtual void update() = 0;   
    
    virtual void add(const edaNeighbour& neighb) = 0;  

    virtual const char* className() const = 0; 

    virtual edaTabuList* clone() const = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;   
    
    virtual bool check(const edaNeighbour& neighb) = 0;
    
    virtual void remove(const edaNeighbour& neighb) = 0;
    
};

#endif	/* EDATABULIST_H */

