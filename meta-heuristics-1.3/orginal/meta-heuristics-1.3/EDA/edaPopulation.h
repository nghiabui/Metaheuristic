/* 
 * File:   edaPopulation.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 7:00 PM
 */

#ifndef EDAPOPULATION_H
#define	EDAPOPULATION_H

#include "edaSolution.h"
#include <algorithm>
#include <vector>
#include <cmath>


class edaPopulation: public edaSerialize {
public:
    // Construction
    edaPopulation() {}
    
    virtual ~edaPopulation() 
    {
        easer();
    }
    
    // Method       
    virtual void add(const edaSolution& sol) 
    {
        _list.push_back(sol.clone());
    }

    virtual void add(const edaSolution* sol) 
    {
        _list.push_back(sol->clone());
    }
    
    virtual void generate(const edaSolution& sol, unsigned int n) 
    {
        easer();        
        for(unsigned int i = 0; i < n; i++) {               
            _list.push_back(sol.clone());
            _list[i]->init();
        }
    }
    
    virtual edaSolution* at(unsigned int index) const
    {
        if( index > _list.size() )
            throw edaException(this, "The index is larger than the number solution !");
        return _list[index];
    }
    
    virtual unsigned int size() const
    {
        return _list.size();
    }
    
    virtual bool isNull() const
    {
        return _list.size() == 0;
    }
    
    virtual void sort() 
    {
        std::sort(_list.begin(),_list.end(), __comparator);
    }
    
    virtual double max() const
    {
        if( isNull() ) 
        {
            return NAN;
        }
        return _list[best()]->evaluate();
    }
    
    virtual double min() 
    {
        if( isNull() ) 
        {
            return NAN;
        }
        return _list[worst()]->evaluate();
    }
    
    virtual double mean() 
    {
        if( isNull() ) 
        {
            return NAN;
        }
        unsigned int size = _list.size();
        double mean = 0;        
        for(unsigned int i = 0; i < size; i++) 
        {
            mean += _list[i]->evaluate();
        }
        mean /=  size;
        return mean;
    }
    
    virtual double std() 
    {
        if( isNull() ) 
        {
            return NAN;
        }
        
        unsigned int size = _list.size();
        double mean = this->mean();
        double std = 0;       
        for(unsigned int i = 0; i < size; i++) 
        {
            std += pow( _list[i]->evaluate() - mean, 2 );
        }
        std = sqrt( std/size );
        return std;
    }
    
    virtual unsigned int best() const 
    {
        if( isNull() ) 
        {
            return -1;
        }
        
        unsigned int size = _list.size();
        unsigned int best = 0;
        
        double bestEval = _list[best]->evaluate();
        
        for(unsigned int i = 1; i < size; i++) 
        {
            double val = _list[i]->evaluate();
            if( bestEval < val ) 
            {
                best = i;
                bestEval = val;
            }
        }
        return best;               
    }
    
    virtual unsigned int worst() const 
    {
        if( isNull() ) 
        {
            return -1;
        }
        
        unsigned int size = _list.size();
        unsigned int worst = 0;
        
        double worstEval = _list[worst]->evaluate();
        
        for(unsigned int i = 1; i < size; i++) 
        {
            double val = _list[i]->evaluate();
            if( worstEval > val ) 
            {
                worst = i;
                worstEval = val;
            }
        }
        return worst;            
    }

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack) 
        {
            unsigned int size = _list.size();
            buf.pack( &size );
            for(unsigned int i = 0; i < size; i++) 
            {
                _list[i]->pack( buf );    
            }
        }    
        else 
        {
            easer();
            unsigned int size;
            buf.unpack( &size );
            _list.resize( size );
            for(unsigned int i = 0; i < size; i++) 
            {
                _list[i] = (edaSolution*) unpack( buf ); 
            }
        }            
    }

    virtual void print(ostream& os) const 
    {
        unsigned int size = _list.size();
        for(unsigned int i = 0; i < size - 1; i++) 
        {
            _list[i]->print(os);
            os << endl;
        }
        _list[size - 1]->print(os);
    }
    
    virtual void easer() 
    {
        easerVector(_list); 
    }

    virtual void easer(unsigned int index) 
    {
        unsigned int size = _list.size();
        if(index > size - 1)
            throw edaException(this, "The index in easer function must be less than the size of population !");
        easerObject(_list[index]);
        _list.erase(_list.begin() + index);
    }
    
    virtual void easer(unsigned int from, unsigned int to) 
    {  
        for(unsigned int i = from; i <= to; i++) 
        {
            easer(from);
        }   
    }
    
    virtual edaSolution* popBack()
    {
        edaSolution* sol = _list.back();
        _list.pop_back();
        return sol;        
    }
    
    virtual edaPopulation* clone() const 
    {
        edaPopulation* pop = new edaPopulation();
        unsigned int size = _list.size();
        for(unsigned int i = 0; i < size; i++) 
        {
            pop->_list.push_back( _list[i]->clone() );
        }
        return pop;
    }
    
    virtual edaSolution* operator[] (unsigned int index) const
    {
        return at(index);
    }    
    
    virtual edaPopulation& operator= (const edaPopulation& pop) 
    {
        easer();
   
        for(unsigned int i = 0; i < pop._list.size(); i++) 
        {
            _list.push_back( pop._list[i]->clone() );
        }
        
        return *this;
    }  
    
    virtual bool operator==(const edaPopulation& pop)
    {
        if( _list.size() == pop._list.size() )
        {
            for(unsigned int i = 0; i < _list.size(); i++)
            {
                if( !(_list[i] == pop._list[i]) )
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAPOPULATION_);  
    
    virtual void init(edaSolution& sol, unsigned int num )
    {
        easer();
        for(unsigned int i = 0; i < num; i++) 
        {
            sol.init();
            add(sol);
        }  
    }
    
    virtual const char* className() const 
    {
        return "edaPopulation";
    }
    
    virtual void reconfig(const edaProblem* pro) 
    {
        return reconfig(*pro);
    }
    
    virtual void reconfig(const edaProblem& pro) 
    {
        std::vector<edaSolution*>::iterator iter;
        for(iter = _list.begin(); iter != _list.end(); iter++ ) 
        {
            edaSolution* sol = *iter;
            sol->set(pro);
        }
    }
    
private:
    std::vector<edaSolution*> _list;

};

#endif	/* EDAPOPULATION_H */

