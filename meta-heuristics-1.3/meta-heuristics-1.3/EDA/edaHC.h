/* 
 * File:   edaHC.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 9:12 PM
 */

#ifndef EDAHC_H
#define	EDAHC_H

#include "edaMoveBase.h"
#include "edaHCExplorer.h"
#include "edaStopCriteria.h"

class edaHC: public edaMoveBase {
public:
    
    edaHC() 
    {
        _crit = NULL;
        _explor = NULL;
    }
    
    edaHC
    (
            const edaNeighbour& neighb, 
            const edaGreedyStrategy& stgy, 
            const edaStopCriteria& crit
    ) {
        set(crit);
        set(edaHCExplorer(neighb, stgy));
    }

    edaHC
    (
            const edaHCExplorer& explor, 
            const edaStopCriteria& crit
    ) {
        set(explor);
        set(crit);
    }
    
    virtual ~edaHC() {
        easer();
    }
    

    void easer() 
    {
        easerObject(_crit);
        easerObject(_explor);  
    }

    virtual void set(const edaStopCriteria& crit) {
        _crit = crit.clone();
    }
    
    virtual void set(const edaHCExplorer& explor) {
        _explor = explor.clone();
    }
    
    virtual void search(edaPopulation& pop) 
    {
        bool converged = true;        
        _crit->init();   
        
        printCSVLog(TaskID, ProcID, "Start", 0, pop);
        printDebug(1,"[" << className() << "]");  
        printDebug(1, "0) " << pop.max()); 
        
        while (_crit->check(pop) && converged)
        {
            converged = _explor->update(pop);
            
            printCSVLog(TaskID, ProcID, "Proc", _crit->loop() , pop);
            printDebug(1, _crit->loop() << ") " << pop.max()); 
            
        }    
            
        printCSVLog(TaskID, ProcID, "Finish", _crit->loop() , pop);
    }
    
    virtual const char* className() const 
    {
        return "edaHC";
    }
    
    virtual edaHC* clone() const  
    {
        checkError();
        edaHC* hc = new edaHC();
        hc->_crit = _crit->clone();
        hc->_explor = _explor->clone();
        return hc;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAHC_ );  
    
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
            _explor = (edaHCExplorer*) unpack(buf);
            _crit = (edaStopCriteria*) unpack(buf);
        }
    }

    virtual void checkError() const
    {
        if(_crit == NULL)
            throw edaException(this, "Not set the stop criteria !");
        if(_explor == NULL)
            throw edaException(this, "Not set the explore strategy !");
    }

protected:
    edaStopCriteria* _crit;
    edaHCExplorer* _explor;
};

#endif	/* EDAHC_H */

