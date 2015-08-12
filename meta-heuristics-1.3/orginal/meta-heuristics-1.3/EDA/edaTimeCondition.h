/* 
 * File:   edaTimeCondition.h
 * Author: Tieu Minh
 *
 * Created on April 13, 2014, 10:45 PM
 */

#ifndef EDATIMECONDITION_H
#define	EDATIMECONDITION_H

#include "edaStopCriteria.h"


class edaTimeCondition : public edaStopCriteria 
{
  
public:
    edaTimeCondition() : _timeCrit(-1) {}
    
    edaTimeCondition(double time_criteria) 
    {
        set(time_criteria);
    }
    
    virtual ~edaTimeCondition() {}
    
    virtual void set(double time_criteria)
    {
        if( time_criteria <= eda::epsilon)        
        {
            throw edaException(this, "The time criteria value must be larger than 0 !");
        }
        
        _timeCrit = time_criteria;
    }
    
    virtual void init()
    {
        checkError();
        _timer.start(true);
        _loop = 0;
        
    }


    virtual const char* className() const
    {
        return "edaTimeCondition";
    }

    
    virtual void checkError() const
    {
        if( _timeCrit <= eda::epsilon)        
        {
            throw edaException(this, "The time criteria must be set the value !");
        }        
    }

    virtual edaTimeCondition* clone() const 
    {
        edaTimeCondition* result = new edaTimeCondition();
        result->set(_timeCrit);
        return result;
    }      

    

    
    virtual unsigned int loop() const 
    {
        return _loop;
    }
    
    virtual bool check(const edaPopulation& pop) 
    {            
        if( _timer.stop() < _timeCrit ) 
        {
            _loop++;
            return true;
        }
        return false;
    }


    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack) 
        {
            buf.pack( &_timeCrit );
        }
        else
        {
            buf.unpack( &_timeCrit );
        }
    }
    
    
    setClassID(_SYSCLASSID_ + _CLSID_EDATIMECONDITION_ ); 
    
protected:
    unsigned int _loop;
    double _timeCrit;
    edaTimer _timer;
};

#endif	/* EDATIMECONDITION_H */

