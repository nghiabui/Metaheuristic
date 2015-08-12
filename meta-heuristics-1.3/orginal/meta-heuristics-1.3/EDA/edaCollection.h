/* 
 * File:   edaCollection.h
 * Author: Tieu Minh
 *
 * Created on April 10, 2014, 9:06 AM
 */

#ifndef EDACOLLECTION_H
#define	EDACOLLECTION_H
#include "edaPopulation.h"

class edaCollection : public edaObject 
{
public:

    edaCollection() {}
    
    virtual ~edaCollection() {}
    
    virtual edaCollection * clone() const = 0;

    virtual void select(edaPopulation &pop) const = 0;

    virtual const char* className() const = 0;
};

#endif	/* EDACOLLECTION_H */

