/* 
 * File:   edaBestImprovement.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDABESTIMPROVEMENT_H
#define	EDABESTIMPROVEMENT_H

#include "edaGreedyStrategy.h"


class edaBestImprovement: public edaGreedyStrategy 
{
public:
    
    edaBestImprovement() 
    {
    }
    
    virtual ~edaBestImprovement() 
    {
    }    

    virtual edaBestImprovement* clone() const 
    {
        return new edaBestImprovement();
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol)
    {
        checkError();         
        
        neighb.set(sol);
        neighb.init();
        
        double best, gain; 
        edaNeighbour* Neighb = neighb.clone();
        edaNeighbour* bestNeighb = neighb.clone(); 
        best = bestNeighb->evaluate();
        
        do 
        {
            gain = neighb.evaluate();
            if( gain > best ) {     
                best = gain;
                easerObject(bestNeighb);
                bestNeighb = neighb.clone(); 
            }   
            neighb.next();   
        } while( !(neighb == *Neighb) );  
        
        if( best > eda::epsilon )
            neighb = *bestNeighb;   
     
        easerObject(Neighb);
        easerObject(bestNeighb);
    }    

    virtual void checkError() const
    {
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDABESTIMPROVEMENT_ );      

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
    }

    virtual const char* className() const
    {
        return "edaBestImprovement";
    }

};

#endif	/* EDABESTIMPROVEMENT_H */

