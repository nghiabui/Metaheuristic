/* 
 * File:   edaRandImprovement.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDARANDIMPROVEMENT_H
#define	EDARANDIMPROVEMENT_H

#include "edaGreedyStrategy.h"


class edaRandImprovement: public edaGreedyStrategy 
{
public:
    
    edaRandImprovement() 
    {
    }
    
    virtual ~edaRandImprovement() 
    {
    }    

    virtual edaRandImprovement* clone() const 
    {
        return new edaRandImprovement();
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) 
    {
        checkError();
        
        neighb.set(sol);
        neighb.rand();
        
        edaNeighbour* Neighb = neighb.clone();
        do 
        {
            if(neighb.evaluate() > eda::epsilon) 
                break;
            neighb.next();
        } while( !(neighb == *Neighb) );  
        easerObject(Neighb);
    }    

    virtual void checkError() const 
    {
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDARANDIMPROVEMENT_ );      

    virtual void serialize(edaBuffer& buf, bool pack) 
    {    
    }


    virtual const char* className() const 
    {
        return "edaRandImprovement";
    }

};

#endif	/* EDARANDIMPROVEMENT_H */

