/* 
 * File:   edaGA.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 8:43 PM
 */

#ifndef EDAGA_H
#define	EDAGA_H

#include "edaPopBase.h"
#include "edaNaturalSelection.h"
#include "edaCrossover.h"
#include "edaMutation.h"
#include "edaStopCriteria.h"


class edaGA : public edaPopBase {
public:
    edaGA() : _slect(NULL), _cross(NULL), _mute(NULL), _crit(NULL)
    {
    }

    edaGA
    (
        const edaNaturalSelection& slect,
        const edaCrossover& cross,
        const edaMutation& mute,
        const edaStopCriteria& crit    
    ) 
        : _slect(NULL), _cross(NULL), _mute(NULL), _crit(NULL)
    {
        set(slect);
        set(cross);
        set(mute);
        set(crit);
    }
    
    virtual ~edaGA() 
    {
        easer();
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
        }
        else
        {
            easer();
            _crit = (edaStopCriteria*) unpack(buf);
            _slect = (edaNaturalSelection *) unpack(buf);
            _cross = (edaCrossover *) unpack(buf);
            _mute = (edaMutation*) unpack(buf);
        }
    }  

    virtual edaGA* clone() const 
    {
        edaGA* ga = new edaGA();
        ga->_slect = _slect->clone();
        ga->_cross = _cross->clone();
        ga->_mute = _mute->clone();
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
    }


    virtual const char* className() const
    {
        return "edaGA";
    }

    setClassID(_SYSCLASSID_ + _CLSID_EDAGA_ ); 
    
protected:
    edaNaturalSelection* _slect;
    edaCrossover* _cross;
    edaMutation* _mute;
    edaStopCriteria* _crit;
};

#endif	/* EDAGA_H */

