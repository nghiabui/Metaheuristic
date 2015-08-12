/* 
 * File:   edaRankSelection.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 3:51 PM
 */

#ifndef EDARANKSELECTION_H
#define	EDARANKSELECTION_H

#include "edaNaturalSelection.h"


class edaRankSelection : public edaNaturalSelection 
{
public:

    edaRankSelection(double rate = 0.5) : _rank(NULL)
    {
        set(rate);
    }
    
    virtual ~edaRankSelection()
    {        
        easer();
    }
    
    virtual void set(double rate)
    {
        if(rate > 1 || rate < 0)
            throw edaException(this, "The rate value must be set in [0, 1] !");
        _rate = rate;        
    }
      
    
    void init(const edaPopulation& pop) 
    {
        easer();
        unsigned int size = pop.size();
        _rank = new double[size];
        double sum = 0;  
        for(unsigned int i = 1; i <= size; ++i)
        {     
            sum += i;        
        }    
        _rank[0] = 1.0 * size / sum;
        for(unsigned int i = 1; i < size; ++i) 
        { 
            _rank[i] = _rank[i-1] + (1.0 * size - i)/sum;    
        }
    }
    
    virtual bool update(edaPopulation& pop)
    {
        pop.sort();    
        init( pop );        
        checkError();
        
        unsigned int size = pop.size();
        unsigned int num = (unsigned int)(_rate*size);

        edaSolution **trash = new edaSolution*[num];
        for(unsigned int i = 0; i < num; i++) 
            trash[num - i - 1] = pop.popBack();
        

        for(unsigned int i = 0; i < num; i++) 
        {
            double prob = eda::rnd.uniform();   
            unsigned int index = 0;
            while(_rank[index] < prob) index++;
            edaSolution* sol = index < size - num ? pop[index] : trash[index - size + num] ;
            pop.add(*sol); 
        }
        easerArrayObject(trash, num);     
        return num < size;
    }

    virtual void serialize(edaBuffer& buf, bool pack)
    {
        if(pack)
        {
            buf.pack(&_rate);
        }
        else
        {
            easer();
            buf.unpack(&_rate);            
        }
    }

    virtual edaRankSelection* clone() const
    {
        edaRankSelection* slect = new edaRankSelection();
        slect->_rate = _rate;
        slect->_rank = NULL;
        return slect;
        
    }
    
    virtual void easer() 
    {
        easerArray(_rank);
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDARANKSELECTION_);
    

    virtual const char* className() const
    {
        return "edaRankSelection";
    }    

    virtual void checkError() const 
    {
        if(eda::isNAN(_rate))
            throw edaException(this, "The rate is not set !");
        if(_rate > 1 || _rate < 0)
            throw edaException(this, "The rate value must be set in [0, 1] !");
    }
    
protected:
    double* _rank;
    double _rate;
    
   
};

#endif	/* EDARANKSELECTION_H */

