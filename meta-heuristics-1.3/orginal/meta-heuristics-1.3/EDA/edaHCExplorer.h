/* 
 * File:   edaHCExplorer.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 7:08 AM
 */

#ifndef EDAHCEXPLORER_H
#define	EDAHCEXPLORER_H

#include "edaGreedyStrategy.h"
#include "edaNeighbour.h"
#include "edaExplorer.h"



class edaHCExplorer: public edaExplorer {
public:
    
    edaHCExplorer() 
    {
        _neighb = NULL;
        _stgy = NULL;
    }
    
    edaHCExplorer(const edaNeighbour& neighb, const edaGreedyStrategy& stgy) 
    {
        set(neighb);
        set(stgy);
    }

    virtual ~edaHCExplorer()
    {
        easer();
    }
    
    
    virtual void set(const edaStrategy& stgy) 
    {
        const edaGreedyStrategy& greedy = (edaGreedyStrategy&)stgy;      
        _stgy = greedy.clone();
    }

    virtual void set(const edaNeighbour& neighb) 
    {
        _neighb = neighb.clone();
    }
    
    virtual void checkError() const 
    {
        if(_stgy == NULL)
            throw edaException(this, "Not set the search strategy !");
        if(_neighb == NULL)
            throw edaException(this, "Not set the neighbor set !");
    }

    virtual edaHCExplorer* clone() const 
    {
        checkError();
        edaHCExplorer* explor = new edaHCExplorer();
        explor->_neighb = _neighb->clone();
        explor->_stgy = _stgy->clone();
        return explor;
    }
    
    virtual void init(const edaPopulation& pop) 
    {
        
    }
    
    virtual bool update(edaPopulation& pop) 
    {
        checkError();
        bool result = false;
        for(unsigned int i = 0; i < pop.size(); i++)
        {                
            edaSolution* sol = pop[i];  
   
            _stgy->explore(*_neighb, *sol);

            if(_neighb->evaluate() > eda::epsilon)   
            {
                _neighb->update(*sol);
                result = true;
            }
        }
        return result;
    }      

    virtual void easer()
    {
        easerObject(_neighb);
        easerObject(_stgy);
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAHCEXPLORER_ );      

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack) 
        {
            _neighb->pack(buf);
            _stgy->pack(buf);
        } 
        else 
        {
            easer();
            _neighb = (edaNeighbour*) unpack(buf);
            _stgy = (edaGreedyStrategy*) unpack(buf);
        }        
    }

    virtual const char* className() const 
    {
        return "edaHCExplorer";
    }

    
    
protected:  
    edaGreedyStrategy* _stgy; 
    edaNeighbour* _neighb;


};

#endif	/* EDAHCEXPLORER_H */

