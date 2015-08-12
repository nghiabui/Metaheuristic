/* 
 * File:   edaMA.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 8:43 PM
 */

#ifndef EDAMA_H
#define	EDAMA_H

#include "edaPopBase.h"
#include "edaNaturalSelection.h"
#include "edaCrossover.h"
#include "edaMutation.h"
#include "edaStopCriteria.h"
#include "edaAdaption.h"


class edaMA : public edaPopBase 
{
public:
    edaMA() : _slect(NULL), _cross(NULL), _mute(NULL), _adapt(NULL) ,_crit(NULL)
    {
    }

    edaMA
    (
        const edaNaturalSelection& slect,
        const edaCrossover& cross,
        const edaMutation& mute,
        const edaAdaption& adapt,
        const edaStopCriteria& crit    
    ) 
        : _slect(NULL), _cross(NULL), _mute(NULL), _adapt(NULL), _crit(NULL)
    {
        set(slect);
        set(cross);
        set(mute);
        set(crit);
        set(adapt);
    }
    
    virtual ~edaMA() 
    {
        easer();
    }
    
    virtual void set(const edaAdaption& adapt)
    {        
        adapt.checkError();
        easerObject(_adapt);
        _adapt = adapt.clone();
    }
    
    virtual void set(const edaStopCriteria& crit)
    {        
        crit.checkError();
        easerObject(_crit);
        _crit = crit.clone();
    }
    
    virtual void set(const edaCrossover& cross)
    {
        cross.checkError();
        easerObject(_cross);
        _cross = cross.clone();
    }
    
    virtual void set(const edaMutation& mute)
    {
        mute.checkError();
        easerObject(_mute);
        _mute = mute.clone(); 
    }    
    
    virtual void set(const edaNaturalSelection& slect)
    {
        slect.checkError();
        easerObject(_slect);
        _slect = slect.clone();         
    }  
    
    virtual void easer() 
    {
        easerObject(_slect);
        easerObject(_cross);
        easerObject(_mute);
        easerObject(_adapt);
        easerObject(_crit);
    }
    
    virtual void search(edaPopulation& pop) 
    {
        _crit->init();   
        
        printCSVLog(TaskID, ProcID, "Start", 0, pop);
        printDebug(1,"[" << className() << "]");  
        printDebug(1, "0) " << pop.max()); 
        
        while (_crit->check(pop))
        {
            _slect->update(pop);
            
            _cross->update(pop);
            
            _mute->update(pop);
            
            _adapt->update(pop);
            
            printCSVLog(TaskID, ProcID, "Proc", _crit->loop() , pop);
            printDebug(1, _crit->loop() << ") " << pop.max()); 
            
        }        
            
        printCSVLog(TaskID, ProcID, "Finish", _crit->loop() , pop);
    }

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack)
        {
            _crit->pack( buf );
            _slect->pack( buf );
            _cross->pack( buf );
            _mute->pack( buf );
            _adapt->pack( buf );
        }
        else
        {
            easer();
            _crit = (edaStopCriteria*) unpack(buf);
            _slect = (edaNaturalSelection *) unpack(buf);
            _cross = (edaCrossover *) unpack(buf);
            _mute = (edaMutation*) unpack(buf);
            _adapt = (edaAdaption*) unpack(buf);
        }
    }  

    virtual edaMA* clone() const 
    {
        edaMA* ga = new edaMA();
        ga->_slect = _slect->clone();
        ga->_cross = _cross->clone();
        ga->_mute = _mute->clone();
        ga->_adapt = _adapt->clone();
        ga->_crit = _crit->clone();
        return ga;
    }
    

    virtual void checkError() const
    {
        if(_crit == NULL)
            throw edaException(this, "Not set the stop criteria !");
        if(_cross == NULL)
            throw edaException(this, "Not set the crossover strategy !");
        if(_mute == NULL)
            throw edaException(this, "Not set the mutation strategy !");
        if(_slect == NULL)
            throw edaException(this, "Not set the natural selection strategy !");
        if(_adapt == NULL)
            throw edaException(this, "Not set the adaption strategy !");
    }


    virtual const char* className() const
    {
        return "edaMA";
    }

    setClassID(_SYSCLASSID_ + _CLSID_EDAMA_ ); 
    
protected:
    edaNaturalSelection* _slect;
    edaCrossover* _cross;
    edaMutation* _mute;
    edaAdaption* _adapt;
    edaStopCriteria* _crit;
};

#endif	/* EDAMA_H */

