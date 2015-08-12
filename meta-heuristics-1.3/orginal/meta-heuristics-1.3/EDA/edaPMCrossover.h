/* 
 * File:   edaPMCrossover.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 7:22 PM
 */

#ifndef EDAPMCROSSOVER_H
#define	EDAPMCROSSOVER_H


#include "edaCrossover.h"
#include <algorithm>

class edaPMCrossover : public edaCrossover
{
public:
    
    edaPMCrossover
    (
        double rate = 1.0, 
        unsigned int numberCrossPoint = 1
    ) 
    { 
        set(rate);
        set(numberCrossPoint);
    }
    
    virtual ~edaPMCrossover()
    {        
    }
    
    virtual void set(double rate)
    {
        if(rate > 1 || rate < 0)
            throw edaException(this, "The rate value must be set in [0, 1] !");
        _rate = rate;        
    }
    
    virtual void set(unsigned int numberCrossPoint)
    {
        if(numberCrossPoint == 0)
            throw edaException(this, "The number crossover point must be larger 0 !");
        _num = numberCrossPoint;
    }
    
    virtual bool update(edaPopulation& pop) 
    {
        checkError();
        bool result = false;
        
        vector<edaSolution*> trash;  
        unsigned int size = pop.size();
        
        for(unsigned int i = 1; i < size; i++) 
        {
            double prob = eda::rnd.uniform();
            if(prob <= _rate) 
            {            
                trash.push_back(pop[i]);           
                if(trash.size() == 2) 
                {               
                    edaChromosome* A = trash[0]->encode();
                    edaChromosome* B = trash[1]->encode();
                    
                    unsigned int length = A->length();
                    bool update = false;
                    
                    if( B->length() != length )
                        throw edaException(this, "The number of gennes in two chromosomes are not equal !");
                    if( _num > length/2 )
                        throw edaException(this, "The number crossover points are large than"
                                " a half of the  number of gennes in chromosome !");
                    
                    unsigned int* cross_points = new unsigned int[_num + 1];    
                    for(unsigned int j = 0; j < _num; j++) 
                    {
                        cross_points[j] = eda::rnd.random( length );                
                    }
                    cross_points[_num] = length;
                     
                    sort(cross_points, cross_points + _num + 1);
                    for(unsigned int j = 0; j < _num; j+= 2) 
                    {
                        for(unsigned int k = cross_points[j]; k < cross_points[j+1]; k++) 
                        {
                            edaGenne *a = A->at(k);
                            edaGenne *b = B->at(k);
                            if(*a != *b) 
                            {
                                edaGenne::swap(a, b);
                                update = true;
                            }
                        }                
                    }
                    
                    vector<unsigned int> stack_1;
                    bool *flag_1 = new bool[length]; 
                    fill(flag_1, flag_1+length, 0);
                    for(unsigned int k = 0; k < length - 1; k++) 
                    {
                        if(!flag_1[k]) 
                        {
                            edaGenne *a1 =  A->at(k);
                            for(unsigned int j = k + 1; j < length; j++) 
                            {
                                if(!flag_1[j]) 
                                {
                                    edaGenne *a2 =  A->at(j);
                                    if(*a1 == *a2) 
                                    {
                                        stack_1.push_back(j);
                                        flag_1[j] = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    
                    vector<unsigned int> stack_2;
                    bool *flag_2 = new bool[length];
                    fill(flag_2, flag_2+length, 0);    
                    for(unsigned int k = 0; k < length - 1; k++) 
                    {
                        if(!flag_2[k]) 
                        {
                            edaGenne *b1 =  B->at(k);
                            for(unsigned int j = k + 1; j < length; j++) 
                            {
                                if(!flag_2[j]) 
                                {
                                    edaGenne *b2 =  B->at(j);
                                    if(*b1 == *b2) 
                                    {
                                        stack_2.push_back(j);
                                        flag_2[j] = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if(stack_1.size() != stack_2.size()) 
                        throw edaException(this, "The solution is not Permutation route !");
                                        

                    for(unsigned int k = 0; k < stack_1.size();k++) 
                    {
                        edaGenne *a = A->at(stack_1[k]);
                        edaGenne *b = B->at(stack_2[k]);
                        edaGenne::swap(a, b);
                    }

                    easerArray(flag_1);
                    easerArray(flag_2);

                    if(update) 
                    {
                        trash[0]->decode(*A);
                        trash[1]->decode(*B);
                        result = true;
                    }   
                    easerArray(cross_points);
                    easerObject(A);
                    easerObject(B);                    
                }
            }
        }
        return result;
    }

    virtual void serialize(edaBuffer& buf, bool pack)
    {
        if(pack)
        {
            buf.pack(&_rate);
            buf.pack(&_num);
        }
        else
        {
            easer();
            buf.unpack(&_rate);  
            buf.unpack(&_num);
        }
    }

    virtual edaPMCrossover* clone() const 
    {
        edaPMCrossover* pmx = new edaPMCrossover();
        pmx->_num = _num;
        pmx->_rate = _rate;
        return pmx;        
    }

    virtual void checkError() const
    {
        if(eda::isNAN(_rate))
            throw edaException(this, "The rate is not set !");
        if(_rate > 1 || _rate < 0)
            throw edaException(this, "The rate value must be set in [0, 1] !");
    }

    virtual const char* className() const
    {
        return "edaPartiallyMatchedCrossover";
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAPMCROSSOVER_);

protected:
    unsigned int _num;
    double _rate;
};

#endif	/* EDAPMCROSSOVER_H */

