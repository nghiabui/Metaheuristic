/* 
 * File:   edaCondition.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 9:21 PM
 */

#ifndef EDASTOPCRITERIA_H
#define	EDASTOPCRITERIA_H

#include "edaPopulation.h"


class edaStopCriteria: public edaSerialize 
{
public:
    edaStopCriteria() {}

    virtual ~ edaStopCriteria() {}
    
    virtual bool check(const edaPopulation& pop) = 0;    
    
    bool check(const edaPopulation* pop) 
    {
        return check(*pop);
    }

    virtual void serialize(edaBuffer& buf, bool pack) = 0;

    virtual edaStopCriteria* clone() const = 0;

    virtual void init() = 0;
    
    virtual unsigned int loop() const = 0;
    
    virtual const char* className() const = 0;    
    
};

#endif	/* EDASTOPCRITERIA_H */

