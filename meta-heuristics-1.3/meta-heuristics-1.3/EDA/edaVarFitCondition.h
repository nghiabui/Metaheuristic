/* 
 * File:   edaVarFitCondition.h
 * Author: Tieu Minh
 *
 * Created on April 13, 2014, 10:53 PM
 */

#ifndef EDAVARTFITCONDITION_H
#define	EDAVARTFITCONDITION_H

#include "edaStopCriteria.h"


class edaVarFitCondition : public edaStopCriteria 
{
public:

    
    edaVarFitCondition()
    {
        _fitness = NAN;
    }
    
    virtual ~edaVarFitCondition() 
    {
    }
    
    virtual void init()
    {
        checkError();
        _loop = 0;
    }
    
    virtual bool check(const edaPopulation& pop)
    {  
        double fit = pop.max();
        if(_loop == 0) 
        {
            _fitness = fit;
        }                    
        else if(_fitness < fit) 
        {
            _fitness = fit;
        }
        else 
        {
            return false;
        }
        _loop ++;
        return true;
    }

    virtual void checkError() const
    {
    }    
    
    virtual const char* className() const 
    {
        return "edaVarFitness";
    }

    virtual edaVarFitCondition* clone() const 
    {
        edaVarFitCondition* result = new edaVarFitCondition() ;
        result->_fitness = _fitness;
        return result;
    }

    virtual unsigned int loop() const 
    {
        return _loop;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAVARFITCONDITION_ ); 


    virtual void serialize(edaBuffer& buf, bool pack) 
    {
    }

    
protected:    
    unsigned int _loop;
    double _fitness;


};

#endif	/* EDAVARTFITCONDITION_H */

