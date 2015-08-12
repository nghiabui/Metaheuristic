/* 
 * File:   edaConstFitCondition.h
 * Author: Tieu Minh
 *
 * Created on April 13, 2014, 10:53 PM
 */

#ifndef EDACONSTFITCONDITION_H
#define	EDACONSTFITCONDITION_H

#include "edaStopCriteria.h"


class edaConstFitCondition : public edaStopCriteria 
{
public:
    
    edaConstFitCondition(double fitness_criteria = NAN) 
    {
        set(fitness_criteria);
    }
    
    virtual ~edaConstFitCondition() {}
    
    virtual void init()
    {
        checkError();
        _loop = 0;
    }
    
    void set(double fitness_criteria)
    {
        if( eda::isNAN(fitness_criteria) ) 
        {
            throw edaException(this, "The fitness criteria value must be set differ with NAN !");
        }
        
        _fitCrit = fitness_criteria;
    }
    
    virtual bool check(const edaPopulation& pop)
    {
        if(pop.max() < _fitCrit)
        {
            _loop++;
            return true;
        }
        return false;      
    }

    virtual void checkError() const
    {
        if( eda::isNAN(_fitCrit) )
        {
            throw edaException(this, "The fitness criteria must be set the value !");                    
        }
    }    
    
    virtual const char* className() const 
    {
        return "edaConstFitness";
    }

    virtual edaConstFitCondition* clone() const 
    {
        edaConstFitCondition* result = new edaConstFitCondition() ;
        result->_fitCrit = _fitCrit;
        return result;
    }

    virtual unsigned int loop() const 
    {
        return _loop;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDACONSTFITCONDITION_ ); 


    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack) 
        {
            buf.pack( &_fitCrit );
        }
        else
        {
            buf.unpack( &_fitCrit );
        }
    }

    
protected:    
    unsigned int _loop;
    double _fitCrit;

private:

};

#endif	/* EDACONSTFITCONDITION_H */

