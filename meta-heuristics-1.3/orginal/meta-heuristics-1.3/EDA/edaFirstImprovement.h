/* 
 * File:   edaFirstImprovement.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDAFIRSTIMPROVE_H
#define	EDAFIRSTIMPROVE_H

#include "edaGreedyStrategy.h"


class edaFirstImprovement: public edaGreedyStrategy 
{
public:
    
    edaFirstImprovement() 
    {
    }
    
    virtual ~edaFirstImprovement() 
    {
    }    

    virtual edaFirstImprovement* clone() const 
    {
        return new edaFirstImprovement();
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) 
    {   
        checkError();
        
        neighb.set(sol);
        neighb.init();
        
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
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAFIRSTIMPROVEMENT_ );      

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
    }

    virtual const char* className() const 
    {
        return "edaFirstImprovement";
    }

};

#endif	/* EDAFIRSTIMPROVE_H */

