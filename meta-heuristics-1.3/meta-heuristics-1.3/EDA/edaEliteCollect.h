/* 
 * File:   edaEliteCollect.h
 * Author: Tieu Minh
 *
 * Created on August 22, 2013, 8:14 AM
 */

#ifndef EDAELITECOLLECT_H
#define	EDAELITECOLLECT_H

#include "edaCollection.h"


class edaEliteCollect : public edaCollection {
public:

    edaEliteCollect(unsigned int number_elite = 1) 
    {        
        _num = number_elite;
    }    

    virtual ~edaEliteCollect() 
    {
    }
    
    virtual edaEliteCollect * clone() const 
    {
        return new edaEliteCollect(_num);
    }
    
    virtual void select(edaPopulation &pop) const 
    {  
        pop.sort();                  
        pop.easer(_num, pop.size() - 1);
    }

    virtual const char* className() const 
    {
        return "edaEliteCollect";
    }

    
protected:
    unsigned int _num;

};

#endif	/* EDAELITECOLLECT_H */

