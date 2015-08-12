/* 
 * File:   edaSAExplorer.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 7:08 AM
 */

#ifndef EDASAEXPLORER_H
#define	EDASAEXPLORER_H

#include "edaCoolingSchedule.h"
#include "edaNeighbour.h"
#include "edaExplorer.h"


class edaSAExplorer: public edaExplorer {
public:
    
    edaSAExplorer() : _stgy(NULL), _neighb(NULL)
    {
    }

    edaSAExplorer
    (
        const edaNeighbour& neighb, 
        const edaCoolingSchedule & stgy
    ) 
        : _stgy(NULL), _neighb(NULL)
    {
        set(neighb);
        set(stgy);
    }
    
    virtual ~edaSAExplorer() 
    {
        easer();
    }
    
    virtual void set(const edaStrategy& stgy) 
    {        
        easerObject(_stgy);
        const edaCoolingSchedule& cool = (edaCoolingSchedule&)stgy;      
        _stgy = cool.clone();
    }

    virtual void set(const edaNeighbour& neighb) 
    {
        easerObject(_neighb);
        _neighb = neighb.clone();
    }
    
    virtual void checkError() const 
    {        
        if(_stgy == NULL)
            throw edaException(this, "Not set the search strategy !");
        if(_neighb == NULL)
            throw edaException(this, "Not set the neighbor set !");
    }

    
    virtual void init(const edaPopulation& pop) 
    {
        
    }

    virtual edaSAExplorer* clone() const 
    {
        edaSAExplorer* explor = new edaSAExplorer();
        explor->_neighb = _neighb->clone();
        explor->_stgy = _stgy->clone();
        return explor;
    }    
    
    virtual bool update(edaPopulation& pop) 
    {
        checkError();
        bool result = false;
        for(unsigned int i = 0; i < pop.size(); i++) 
        {
            edaSolution* sol = pop[i];  
            _stgy->init();             
            do 
            {
                edaNeighbour* neighb = _neighb->clone();
                neighb->set(*sol);
                
                _stgy->explore(*_neighb, *sol);
                
                if(_neighb->evaluate() > eda::epsilon)   
                {
                    result = true;
                }
                
                if(! (*neighb == *_neighb) )
                {
                    _neighb->update(*sol);
                }
                
                easerObject(neighb);
                
            } while( _stgy->equil() );            
        }
        return result;
    }      

    virtual void easer() 
    {
        easerObject(_neighb);
        easerObject(_stgy);
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDASAEXPLORER_ );  
    
    virtual const char* className() const
    {
        return "edaSAExplorer";
    }

    
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
            _stgy = (edaCoolingSchedule*) unpack(buf);
        }   
    }

protected:    
    edaCoolingSchedule* _stgy; 
    edaNeighbour* _neighb;


};

#endif	/* EDASAEXPLORER_H */

