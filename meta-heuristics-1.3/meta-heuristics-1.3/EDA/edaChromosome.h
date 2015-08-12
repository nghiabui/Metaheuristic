/* 
 * File:   edaChromosome.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 9:49 PM
 */

#ifndef EDACHROMOSOME_H
#define	EDACHROMOSOME_H

#include "edaObject.h"
#include "edaGenne.h"


class edaChromosome : public edaObject
{
public:
    edaChromosome()
    {
    }
    
    unsigned int length() const
    {
        return _gennes.size();
    }
    
    virtual ~edaChromosome()
    {
        easer();
    }
    
    edaGenne* at(unsigned int index) const
    {
        if( index >= _gennes.size() )
            throw edaException(this, "The index value is lager than"
                    " the number of gennes in chromosome !");
        return _gennes[index];
    }
    
    void add(const edaGenne& genne)
    {
        genne.checkError();
        _gennes.push_back(genne.clone());
    }
    
    edaGenne* operator[] (unsigned int index) const
    {
        return at(index); 
    }
    
    bool operator != (const edaChromosome &chro) const
    {
        return !operator == (chro);
    }
    
    bool operator == (const edaChromosome &chro) const
    {
        unsigned int size = _gennes.size();
        if( size !=  chro._gennes.size() )
            return false;
        for(unsigned int i = 0; i < size; i++)
        {
            if(*_gennes[i] != *chro._gennes[i] )
                return false;
        }
        return true;
    }
    
    void checkError() const
    {
        unsigned int size = _gennes.size();
        if( size == 0 )
            throw edaException(this, "The chromosome has no genne !");
        
        for(unsigned int i = 0; i < size; i++ )
        {
            _gennes[i]->checkError();
        }
    }
    
    void easer()
    {
        easerVector(_gennes);
    }

    const char* className() const
    {
        return "edaChromosome";
    }

    virtual void print(ostream &os) const 
    {
        unsigned int size = _gennes.size();
        os << *_gennes[0];
        for(unsigned int i = 1; i < size; i++)
        {
            os << " " << *_gennes[i];
        }
    }
    
    
private:
    vector<edaGenne*> _gennes;
};

#endif	/* EDACHROMOSOME_H */

