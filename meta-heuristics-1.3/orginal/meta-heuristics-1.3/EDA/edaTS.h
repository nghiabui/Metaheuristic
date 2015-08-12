/* 
 * File:   edaTS.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 8:44 PM
 */

#ifndef EDATS_H
#define	EDATS_H

#include "edaMoveBase.h"
#include "edaStopCriteria.h"
#include "edaTSExplorer.h"


class edaTS : public edaMoveBase {
public:
    
    edaTS() 
    {
        _explor = NULL;
        _crit = NULL;
    }
    
    edaTS(const edaTSExplorer& explor, const edaStopCriteria& crit ) 
    {
        set(crit);
        set(explor);
    }
    
    edaTS(const edaNeighbour& neighb, const edaAspirCrit& aspir , const edaTabuList& tabu, const edaStopCriteria& crit ) 
    {
        set(crit);
        set( edaTSExplorer(neighb, aspir, tabu) );
    }
    
    virtual ~edaTS() {
        easer();
    }
    
    virtual void easer() 
    {
        easerObject(_crit);
        easerObject(_explor);  
    }
    
    virtual void set(const edaStopCriteria& crit) {
        _crit = crit.clone();
    }
    
    virtual void set(const edaTSExplorer& explor) {
        _explor = explor.clone();
    }
    
    virtual edaTS* clone() const
    {
        edaTS* ts = new edaTS();
        ts->_crit = _crit->clone();
        ts->_explor = _explor->clone();
        return ts;
    }    

    virtual const char* className() const
    {
        return "edaTS";
    }
    
    virtual void checkError() const 
    {
        if(_crit == NULL)
            throw edaException(this, "Not set the stop criteria !");
        if(_explor == NULL)
            throw edaException(this, "Not set the explore strategy !");
    }
    

    virtual void search(edaPopulation& pop) 
    {  
        _crit->init();   
        _explor->init(pop);
        
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
    
    setClassID(_SYSCLASSID_ + _CLSID_EDATS_ );  
    
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
            _explor = (edaTSExplorer*) unpack(buf);
            _crit = (edaStopCriteria*) unpack(buf);
        }
    }
    
protected:
    edaTSExplorer* _explor;
    edaStopCriteria* _crit;
};

#endif	/* EDATS_H */

