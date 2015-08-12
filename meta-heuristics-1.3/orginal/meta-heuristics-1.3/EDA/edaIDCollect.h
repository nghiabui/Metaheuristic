/* 
 * File:   edaIDCollect.h
 * Author: Tieu Minh
 *
 * Created on April 24, 2014, 7:15 PM
 */

#ifndef EDAIDCOLLECT_H
#define	EDAIDCOLLECT_H

#include "edaCollection.h"


class edaIDCollect : public edaCollection
{
public:
    edaIDCollect() : _id(eda::FLAG)
    {
    }
    
    edaIDCollect(unsigned int id) 
    {
        _id = id;
    }

    virtual ~edaIDCollect() 
    {
    }

    virtual void set(unsigned int id)
    {
        _id = id;
    }
    
    virtual edaIDCollect * clone() const 
    {
        return new edaIDCollect(_id);
    }

    virtual void select(edaPopulation &pop) const
    {
        //Checking the invalid list solution 
        if(_id  >= pop.size() )
            throw edaException(this, "The index larger than population size !");

        //Select the solution
        edaSolution *sol = pop[_id]->clone();
        pop.easer();
        pop.add(sol);
    }

    virtual const char* className() const 
    {
        return "edaIDCollect";
    }

    
protected:
    unsigned int _id;  

};

#endif	/* EDAIDCOLLECT_H */

