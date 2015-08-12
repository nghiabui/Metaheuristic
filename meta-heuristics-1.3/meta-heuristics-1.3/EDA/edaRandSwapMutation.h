/* 
 * File:   edaRandSwapMutation.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 7:21 PM
 */

#ifndef EDARANDSWAPMUTATION_H
#define	EDARANDSWAPMUTATION_H

#include "edaMutation.h"


class edaRandSwapMutation : public edaMutation
{
public:
 
    edaRandSwapMutation(double rate = 0.05)
    {        
        _rate = rate;
    }
    
    virtual ~edaRandSwapMutation()
    {        
    }
    
    virtual bool update(edaPopulation& pop) 
    {
        checkError();
        bool result = false;
        
        unsigned int size = pop.size();        
        for(unsigned int i = 1; i < size; i++) 
        {
            double prob = eda::rnd.uniform();
            unsigned int a, b;
            if( prob <= _rate ) 
            { 
                edaSolution* sol = pop[i];
                edaChromosome* chro = sol->encode();
                unsigned int num = chro->length();
                a = eda::rnd.random (num);
                b = eda::rnd.random (num);
                edaGenne *A = chro->at(a);
                edaGenne *B = chro->at(b);
                if(*A != *B) 
                {
                    edaGenne::swap(A, B);
                    sol->decode(*chro);
                    result = true;
                }
                easerObject(chro);
            }
        }
        return result;
    }

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if (pack)
        {
            buf.pack( &_rate );
        }
        else
        {
            buf.unpack( &_rate );
        }        
    }

    virtual edaRandSwapMutation* clone() const 
    {
        edaRandSwapMutation* mute = new edaRandSwapMutation();
        mute->_rate = _rate;
        return mute;
    }
    

    virtual void checkError() const
    {
        if(eda::isNAN(_rate))
            throw edaException(this, "The rate is not set !");
    }


    virtual const char* className() const
    {
        return "edaRandSwapMutation";
    }

    
    setClassID(_SYSCLASSID_ + _CLSID_EDARANDSWAPMUTATION_);
    
protected:
    void getRandPermutation(edaChromosome *chro) const;
    double _rate;
};

#endif	/* EDARANDSWAPMUTATION_H */

