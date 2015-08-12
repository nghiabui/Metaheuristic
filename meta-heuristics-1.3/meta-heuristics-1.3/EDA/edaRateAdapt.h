/* 
 * File:   edaRateAdapt.h
 * Author: Tieu Minh
 *
 * Created on April 18, 2014, 12:33 PM
 */

#ifndef EDARATEADAPT_H
#define	EDARATEADAPT_H

#include "edaAdaption.h"


class edaRateAdapt : public edaAdaption
{
public:
    edaRateAdapt() : _lsearch(NULL)
    {
    }
    
    edaRateAdapt
    (
        const edaMoveBase& localSearch, 
        double rate = 1.0
    ) : _lsearch(NULL)
    {
        set(localSearch);
        set(rate);
    }
    
    virtual void set(const edaMoveBase& localSearch)
    {
        localSearch.checkError();
        easerObject(_lsearch);
        _lsearch = localSearch.clone();
    }
    
    virtual void set(double rate)
    {
        if(rate < 0 || rate > 1)
            throw edaException(this, "The rate value must be in [0, 1] !");
        _rate = rate;
    }
    

    virtual bool update(edaPopulation& pop)
    {
        checkError();  
        disableLog();
        _lsearch->search(pop); 
        enableLog();
        return true;
    }

    
    virtual ~edaRateAdapt() 
    {
        easer();
    }
    
    virtual void easer() 
    {
        easerObject(_lsearch);
    }

    virtual void checkError() const
    {
        if(_lsearch == NULL)
            throw edaException(this, "The local search is NULL !");
    }

    virtual edaRateAdapt* clone() const
    {
        return new edaRateAdapt(*_lsearch, _rate);        
    }

    virtual void serialize(edaBuffer& buf, bool pack)
    {
        if(pack)
        {
            _lsearch->pack(buf);
        }
        else
        {
            easer();
            _lsearch = (edaMoveBase*) unpack(buf);
        }
    }
    
    virtual const char* className() const
    {
        return "edaRateAdapt";
    }

    setClassID(_SYSCLASSID_ + _CLSID_EDARATEADAPT_ );    
        
protected:
    edaMoveBase *_lsearch;
    float _rate;
};

#endif	/* EDARATEADAPT_H */

