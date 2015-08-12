/* 
 * File:   edaStaticTabu.h
 * Author: Tieu Minh
 *
 * Created on April 21, 2014, 9:37 AM
 */

#ifndef EDASTATICTABU_H
#define	EDASTATICTABU_H


#include "edaTabuList.h"


class edaStaticTabu : public edaTabuList
{
public:
    
    edaStaticTabu() : _maxSize(eda::FLAG)
    {
    }
    
    edaStaticTabu(unsigned int maxSize)
    {
        set(maxSize);
    }
    
    
    virtual void set(unsigned int maxSize)
    {
        _maxSize  = maxSize;
    }
    
    virtual void init()
    {        
    }
    
    virtual void update() 
    {        
    }    
    
    virtual void add(const edaNeighbour& neighb)
    {
        unsigned int size = _tabu.size();
        
        if(size > 0)
        {
            remove(neighb);                
        }

        _tabu.push_back( neighb.clone() );
        if (size == _maxSize)
        {
            easerObject( _tabu.front() );
            _tabu.erase( _tabu.begin() );
        }    
    }    

    virtual const char* className() const
    {
        return "edaStaticTabu";
    }    

    virtual edaStaticTabu* clone() const 
    {
        edaStaticTabu* tabuList = new edaStaticTabu();
        unsigned int size = _tabu.size();
        for(unsigned int i = 0; i < size; i++)
        {
            tabuList->_tabu.push_back( _tabu[i]->clone() );
        }
        tabuList->_maxSize = _maxSize;
        return tabuList;
    }

    virtual void serialize(edaBuffer& buf, bool pack)
    {
        unsigned int size;
        if(pack)
        {
            size = _tabu.size();
            buf.pack( &size );
            for(unsigned int i = 0; i < size; i++)
            {
                edaNeighbour* neighb = _tabu[i];
                neighb->pack(buf);
            }    
            
            buf.pack( &_maxSize );
        }
        else
        {
            easer();
            
            buf.unpack( &size );            
            for(unsigned int i = 0; i < size; i++)
            {
                edaNeighbour* neighb = (edaNeighbour* ) unpack(buf);
                _tabu.push_back(neighb);
            }  
            
            buf.unpack( &_maxSize );
        }
    }

    virtual void print(ostream& os) const 
    {
        unsigned int size = _tabu.size();
        for(unsigned int i = 0; i < size - 1; i++) 
        {
            _tabu[i]->print(os);
            os << endl;
        }
        _tabu[size - 1]->print(os);
    }

    virtual void easer()
    {
        easerVector(_tabu);
    }
    
    virtual ~edaStaticTabu()
    {
        easer();
    }
    
    virtual bool check(const edaNeighbour& neighb)
    {
        unsigned int size = _tabu.size();
        for(unsigned int i = 0; i < size; i++)
        {
            if( neighb == *(_tabu[i]) )
                return true;
        }
        return false;
    }
    
    virtual void remove(const edaNeighbour& neighb) 
    {
        unsigned int size = _tabu.size();
        for(unsigned int i = 0; i < size; i++)
        {
            if( neighb == *_tabu[i] )
            {
                easerObject(_tabu[i]);
                _tabu.erase(_tabu.begin() + i);
            }
        }
    }    

    setClassID(_SYSCLASSID_ + _CLSID_EDASTATICTABU_ ); 
    
protected:
    vector<edaNeighbour *> _tabu;
    unsigned int _maxSize;
};

#endif	/* EDASTATICTABU_H */

