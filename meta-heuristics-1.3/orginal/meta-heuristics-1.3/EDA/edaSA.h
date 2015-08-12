/* 
 * File:   edaSA.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 6:37 AM
 */

#ifndef EDASA_H
#define	EDASA_H

#include "edaMoveBase.h"
#include "edaStopCriteria.h"
#include "edaSAExplorer.h"

class edaSA: public edaMoveBase {
public:
    
    edaSA()
    {
        _explor = NULL;
        _crit = NULL;
    }
    
    edaSA
    (
        const edaNeighbour& neighb, 
        const edaCoolingSchedule& sched, 
        const edaStopCriteria& crit
    ) {
        set(crit);
        set(edaSAExplorer(neighb, sched));
    }
    
    edaSA
    (
        const edaSAExplorer& explor, 
        const edaStopCriteria& crit 
    ) {
        set(crit);
        set(explor);
    }
    
    virtual ~edaSA() 
    {
        easer();
    }
    
    virtual void set(const edaStopCriteria& crit) {
        _crit = crit.clone();
    }
    
    virtual void set(const edaSAExplorer& explor) {
        _explor = explor.clone();
    }

    virtual void checkError() const 
    {
        if(_crit == NULL)
            throw edaException(this, "Not set the stop criteria !");
        if(_explor == NULL)
            throw edaException(this, "Not set the explore strategy !");
    }

    virtual void easer() 
    {
        easerObject(_crit);
        easerObject(_explor);  
    }

    virtual void search(edaPopulation& pop) 
    {  
        _crit->init();   
        
        printCSVLog(TaskID, ProcID, "Start", 0, pop);
        printDebug(1,"[" << className() << "]"); 
        printDebug(1, "0) " << pop.max()); 
        
        while (_crit->check(pop))
        {
            _explor->update(pop);
            
            printCSVLog(TaskID, ProcID, "Proc", _crit->loop() , pop);
            printDebug(1, _crit->loop() << ") " << pop.max()); 
            
        }    
            
        printCSVLog(TaskID, ProcID, "Finish", _crit->loop() , pop);
    }
    
    
    virtual edaSA* clone() const  
    {
        edaSA* sa = new edaSA();
        sa->_crit = _crit->clone();
        sa->_explor = _explor->clone();
        return sa;
    }
    
    virtual const char* className() const {
        return "edaSA";
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDASA_ );  
    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        edaSearch::serialize(buf, pack);
        if(pack) 
        {
            _explor->pack(buf);
            _crit->pack(buf);
        }
        else
        {
            easer();
            _explor = (edaSAExplorer*) unpack(buf);
            _crit = (edaStopCriteria*) unpack(buf);
        }
    }
    
protected:
    edaSAExplorer* _explor;
    edaStopCriteria* _crit;
};

#endif	/* EDASA_H */

