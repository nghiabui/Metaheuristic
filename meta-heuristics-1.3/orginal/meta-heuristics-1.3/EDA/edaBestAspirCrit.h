/* 
 * File:   edeBestAspirCrit.h
 * Author: Tieu Minh
 *
 * Created on April 21, 2014, 2:40 PM
 */

#ifndef EDABESTASPIRCRIT_H
#define	EDABESTASPIRCRIT_H

#include "edaAspirCrit.h"
#include "edaGreedyStrategy.h"
#include "edaBestImprovement.h"


class edaBestAspirCrit : public edaAspirCrit {
public:
    edaBestAspirCrit() : _gain( -eda::INF )
    {
        _stgy = new edaBestImprovement();
    }
    
    edaBestAspirCrit(const edaGreedyStrategy& stgy ) 
        :  _stgy(NULL), _gain( -eda::INF )
    {        
        set(stgy);
    }
    
    void set( const edaGreedyStrategy& stgy )
    {
        stgy.checkError();
        easerObject(_stgy);
        _stgy = stgy.clone();
    }
    
    virtual ~edaBestAspirCrit()
    {   
        easer();
    }
    
    virtual void init()
    {
        _gain = NAN;
    }
    
    virtual edaBestAspirCrit* clone() const 
    {
        edaBestAspirCrit* aspir = new edaBestAspirCrit();
        aspir->_gain = _gain;
        aspir->set( *_stgy );
        return aspir;
    }

    virtual void explore(edaNeighbour& neighb, const edaSolution& sol)
    {
        checkError();
        _stgy->explore(neighb, sol);
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
        return "edaBestAspirCrit";
    }
    
    virtual bool check(edaNeighbour& neighb) 
    {
        double gian = neighb.evaluate();
        if (eda::isNAN(_gain))
        {
            _gain = gian;
            return true;
        }
        else
        {
            if( _gain < gian )
            {
                return false;
            }
            else
            {
                _gain = gian;
                return true;
            }
        }
    }    

    virtual void checkError() const
    {
        if(_stgy == NULL )
            throw edaException(this, "The strategy is not set !");
        if(_gain == -eda::INF )
            throw edaException(this, "The edaAspirCrit not initial !");
    }

    virtual void easer() 
    {
        easerObject(_stgy);
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDABESTASPIRCRIT_);  
    
protected:
    edaGreedyStrategy* _stgy; 
    double _gain;
    
};

#endif	/* EDABESTASPIRCRIT_H */

