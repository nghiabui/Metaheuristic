/* 
 * File:   tspNeighbour.h
 * Author: Tieu Minh
 *
 * Created on April 15, 2014, 10:31 AM
 */

#ifndef TSPNEIGHBOUR_H
#define	TSPNEIGHBOUR_H

#include "tspDefine.h"
#include "tspSolution.h"

class tspNeighbour : public edaNeighbour {
public:    
    // Construction
    tspNeighbour() 
    {
        _route = NULL;
        _gain = NAN;
    }
    
    ~tspNeighbour()
    {
        easer();
    }
    
    // Method
    virtual void set(const edaSolution& sol) 
    {  
        tspSolution& route = (tspSolution&) sol;
        route.checkError();
        _route = &route; 

    }
    
    virtual double evaluate() = 0;  
    
    virtual tspNeighbour* clone() const = 0;
    
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
    
    virtual tspNeighbour& operator =(const edaNeighbour& neighb) = 0;
    
    virtual bool operator ==(const edaNeighbour& neighb) const = 0;
    
    virtual const char* className() const = 0;

protected:
    const tspSolution* _route;
    double _gain;
};

#endif	/* TSPNEIGHBOUR_H */

