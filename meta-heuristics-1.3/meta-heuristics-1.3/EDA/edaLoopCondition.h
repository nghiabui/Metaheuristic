/* 
 * File:   edaLoopCondition.h
 * Author: Tieu Minh
 *
 * Created on March 14, 2014, 11:18 PM
 */

#ifndef EDALOOPCONDITION_H
#define	EDALOOPCONDITION_H

#include "edaStopCriteria.h"
#include "edaException.h"
#include "edaBuffer.h"


class edaLoopCondition : public edaStopCriteria {
public:
    edaLoopCondition() : _maxNumLoop(eda::FLAG) {}
    
    edaLoopCondition(unsigned int maxNumLoop) 
    {
        set(maxNumLoop);
    }
    
    void set(unsigned int maxNumLoop) 
    {
        _maxNumLoop = maxNumLoop;
    }
    
    virtual ~edaLoopCondition() {}

    virtual void init()
    {
        checkError();
        _loop = 0;
    }
    

    virtual edaLoopCondition* clone() const {
        edaLoopCondition* con = new edaLoopCondition();
        con->_maxNumLoop = _maxNumLoop;
        return con;
    }


    virtual void checkError() const 
    {
    }    

    virtual bool check(const edaPopulation& pop) 
    {
        if(_maxNumLoop == eda::FLAG) 
        {
            _loop++;
            return true;
        }
        else if(_loop < _maxNumLoop) 
        {
            _loop++;
            return true;
        }
        return false;
    }
    

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack) 
        {
            buf.pack( &_maxNumLoop );
        }
        else
        {
            buf.unpack( &_maxNumLoop );
        }
    }
    
    virtual unsigned int loop() const {
        return _loop;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDALOOPCONDITION_ );  
    
    const char* className() const {
        return "edaLoopCondition";
    }

    protected:
        unsigned int _loop;
        unsigned int _maxNumLoop;
};

#endif	/* EDALOOPCONDITION_H */

