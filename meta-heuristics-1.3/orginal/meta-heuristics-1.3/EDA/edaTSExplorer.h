/* 
 * File:   edaTSExplorer.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 7:08 AM
 */

#ifndef EDATSEXPLORER_H
#define	EDATSEXPLORER_H

#include "edaCoolingSchedule.h"
#include "edaNeighbour.h"
#include "edaExplorer.h"
#include "edaGreedyStrategy.h"
#include "edaAspirCrit.h"
#include "edaTabuList.h"

class edaTSExplorer: public edaExplorer {
public:
    
    edaTSExplorer() : _neighb(NULL)
    {
    }

    edaTSExplorer
    (   
        const edaNeighbour& neighb, 
        const edaAspirCrit& aspir,
        const edaTabuList& tabuList
    ) 
        : _neighb(NULL)
    {
        set(neighb);
        set(aspir);
        set(tabuList);
    }
    
    virtual ~edaTSExplorer() 
    {
        easer();
    }    

    virtual void set(const edaNeighbour& neighb) 
    {
        easerObject(_neighb);
        _neighb = neighb.clone();
    }
    
    virtual void set(const edaStrategy& stgy) 
    {
        easerVector(_stgys);
        const edaAspirCrit& aspir = (edaAspirCrit&)stgy;   
        _stgys.push_back( aspir.clone() );
    }
    
    virtual void set(const edaTabuList& tabu) 
    {
        easerVector(_tabus);
        _tabus.push_back( tabu.clone() );
    }

    
    virtual void checkError() const 
    {        
        if(_neighb == NULL)
            throw edaException(this, "Not set the neighbor set !");
        if(_stgys.size() < 1)
            throw edaException(this, "Not set the aspiration strategy !");
        if(_tabus.size() < 1)
            throw edaException(this, "Not set the tabu list !");
    }

    virtual void init(const edaPopulation& pop)
    {
        checkError();
        _stgys[0]->init();
        unsigned int size = pop.size();     
        for(unsigned int i = 1 ; i < size; i++)
        {
            _stgys.push_back( _stgys[0]->clone() );           
            _tabus.push_back( _tabus[0]->clone() );
        }
    }

    
    virtual edaTSExplorer* clone() const 
    {
        edaTSExplorer* explor = new edaTSExplorer();
        explor->_neighb = _neighb->clone();
        
        for(unsigned int i = 0 ; i < _stgys.size() ; i++)
        {
            explor->_stgys.push_back( _stgys[i]->clone() );
        }
        
        for(unsigned int i = 0 ; i < _tabus.size() ; i++)
        {
            explor->_tabus.push_back( _tabus[i]->clone() );
        }
        
        return explor;
    }    
    
    virtual bool update(edaPopulation& pop) 
    {
        checkError();
        bool result = false;
        for(unsigned int i = 0; i < pop.size(); i++) 
        {
            edaSolution* sol = pop[i];             

            _stgys[i]->explore(*_neighb, *sol);

            if( _stgys[i]->check(*_neighb)  || !_tabus[i]->check(*_neighb) )   
            {
                _neighb->update(*sol);
            }                
        
        }
        return result;
    }      

    virtual void easer() 
    {
        easerObject(_neighb);
        easerVector(_stgys);
        easerVector(_tabus);
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDATSEXPLORER_ );  
    
    virtual const char* className() const
    {
        return "edaTSExplorer";
    }

    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        unsigned int size;
        if(pack) 
        {
            _neighb->pack(buf);    
            
            size = _stgys.size();
            buf.pack( &size );
            for(unsigned int i = 0; i < size; i++)
            {
                _stgys[i]->pack(buf);
            }
            
            size = _tabus.size();
            buf.pack( &size );
            for(unsigned int i = 0; i < size; i++)
            {
                _tabus[i]->pack(buf);
            }
        } 
        else 
        {
            easer();
            _neighb = (edaNeighbour*) unpack(buf);
            
            buf.unpack( &size );
            for(unsigned int i = 0; i < size; i++)
            {
                edaAspirCrit* aspir = (edaAspirCrit*) unpack(buf);
                _stgys.push_back(aspir);
            }
            
            buf.unpack( &size );
            for(unsigned int i = 0; i < size; i++)
            {
                edaTabuList* tabu = (edaTabuList*) unpack(buf);
                _tabus.push_back(tabu);
            }
        }   
    }

protected:    
    edaNeighbour* _neighb;
    vector<edaAspirCrit*> _stgys;
    vector<edaTabuList*> _tabus;


};

#endif	/* EDATSEXPLORER_H */

