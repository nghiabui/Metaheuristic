/* 
 * File:   edaSynthCollect.h
 * Author: Tieu Minh
 *
 * Created on August 22, 2013, 10:30 PM
 */

#ifndef EDASYNTHCOLLECT_H
#define	EDASYNTHCOLLECT_H

#include "edaCollection.h"


class edaSynthCollect : public edaCollection {
public:

    edaSynthCollect() 
    {
    }

    virtual ~edaSynthCollect() 
    {
    }

    virtual edaSynthCollect * clone() const 
    {
        return new edaSynthCollect();
    }

    virtual void select(edaPopulation &pop) const 
    {        
    }
  

    virtual const char* className() const 
    {
        return "edaSynthCollect";
    }

};

#endif	/* EDASYNTHCOLLECT_H */

