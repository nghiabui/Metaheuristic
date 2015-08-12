/* 
 * File:   edaNoAspirCrit.h
 * Author: Tieu Minh
 *
 * Created on April 21, 2014, 2:24 PM
 */

#ifndef EDANOASPIRCRIT_H
#define	EDANOASPIRCRIT_H

#include "edaAspirCrit.h"
#include "edaGreedyStrategy.h"


class edaNoAspirCrit: edaAspirCrit
{
public:
    edaNoAspirCrit()
    {        
        _stgy = new edaBestImprovement();
    }
    
    edaNoAspirCrit( const edaGreedyStrategy& stgy) : _stgy(NULL) 
    {        
        set(stgy);
    }
    
    void set( const edaGreedyStrategy& stgy )
    {
        stgy.checkError();
        easerObject(_stgy);
        _stgy = stgy.clone();
    }
    
    virtual edaNoAspirCrit* clone() const 
    {       
        edaNoAspirCrit* aspir = new edaNoAspirCrit();
        aspir->_stgy = _stgy->clone();
        return aspir;
    }
    
    virtual void easer()
    {
        easerObject(_stgy);
    }
    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if( pack )
        {
            _stgy->pack(buf);
        }
        else
        {
            easer();
            _stgy = (edaGreedyStrategy*) unpack(buf);
        }
    }
    
    virtual const char* className() const 
    {
        return "edaNoAspirCrit";
    }
    
    virtual bool check(edaNeighbour& neighb) 
    {
        return false;
    }
    
    virtual ~edaNoAspirCrit()
    {
        easer();
    }
    
    virtual void init()
    {        
    }

    virtual void checkError() const
    {
        if(_stgy == NULL )
            throw edaException(this, "The strategy is not set !");
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol)
    {
        checkError();
        _stgy->explore(neighb, sol);
    }
    
protected:
    edaGreedyStrategy* _stgy; 
};

#endif	/* EDANOASPIRCRIT_H */

