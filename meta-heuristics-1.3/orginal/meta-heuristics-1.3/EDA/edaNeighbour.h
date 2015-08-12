/* 
 * File:   edaNeighbour.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 4:44 PM
 */

#ifndef EDANEIGHBOUR_H
#define	EDANEIGHBOUR_H

#include "edaSolution.h"


class edaNeighbour: public edaSerialize {
public:
    
    edaNeighbour() {}
    
    virtual ~edaNeighbour() {}
    
    virtual void update(edaSolution& sol) = 0;
    
    virtual void next() = 0;
    
    virtual void set(const edaSolution& sol) = 0;
    
    virtual void init() = 0;
    
    virtual void rand() = 0;
    
    virtual double evaluate() = 0;

    virtual edaNeighbour* clone() const = 0;    
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaNeighbour& operator = (const edaNeighbour& neighb) = 0;
    
    virtual bool operator == (const edaNeighbour& neighb) const = 0;
    
    virtual const char* className() const = 0;

};

#endif	/* EDANEIGHBOUR_H */

