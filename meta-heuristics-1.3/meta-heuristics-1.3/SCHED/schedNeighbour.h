/* 
 * File:   schedNeighbour.h
 * Author: Trung Nghia
 *
 * Created on April 15, 2014, 10:31 AM
 */

#ifndef schedNEIGHBOUR_H
#define	schedNEIGHBOUR_H

#include "schedDefine.h"
#include "schedSolution.h"
#include "schedFitness.h"

class schedNeighbour : public edaNeighbour {
public:    
    // Construction
    schedNeighbour()
    {
        _route = NULL;
        _gain = NAN;
    }
    
    ~schedNeighbour()
    {
        easer();
    }
    
    // Method
    virtual void set(const edaSolution& sol) 
    {  
        schedSolution& route = (schedSolution&) sol;
        route.checkError();
        _route = &route; 

    }
    

    virtual double evaluate()=0;
//    virtual edaFitness* evaluate()=0;
    virtual schedNeighbour* clone() const = 0;
    
    virtual void update(edaSolution& sol) = 0;
    
    virtual void checkError() const
    {
        if(_route == NULL)
            throw edaException(this, "Not set route !");
    }
    
    virtual void next() = 0;
    
    virtual void rand() = 0;
    
    virtual void easer()
    {
        _route = NULL;
        _gain = NAN;
    }
    virtual void init() = 0;
    
    virtual void print(ostream& os) const = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack)
        {
            buf.pack( &_gain );
        }
        else
        {
            buf.unpack( &_gain );
        }
    }
    
    virtual schedNeighbour& operator =(const edaNeighbour& neighb) = 0;
    
    virtual bool operator ==(const edaNeighbour& neighb) const = 0;
    
    virtual const char* className() const = 0;

protected:
    const schedSolution* _route;
    double _gain;
};

#endif	/* schedNEIGHBOUR_H */

