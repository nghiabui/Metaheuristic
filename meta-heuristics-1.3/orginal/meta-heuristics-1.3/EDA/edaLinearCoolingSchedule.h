/* 
 * File:   edaLinearCoolingSchedule.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDALINEARCOOLINGSCHEDULE_H
#define	EDALINEARCOOLINGSCHEDULE_H

#include "edaCoolingSchedule.h"


class edaLinearCoolingSchedule: public edaCoolingSchedule {
public:
    
    edaLinearCoolingSchedule() : _threshold(NAN), _quantity(NAN), _initTemp(NAN) {}
    
    virtual ~edaLinearCoolingSchedule() {
        _threshold = _quantity = _initTemp = NAN;
    }    

    virtual edaLinearCoolingSchedule* clone() const 
    {
        edaLinearCoolingSchedule* cool = new edaLinearCoolingSchedule();
        cool->_threshold = _threshold;
        cool->_initTemp = _initTemp;
        cool->_quantity = _quantity;
        cool->_temp = _temp;
        return cool;
    }
    
    virtual void set(double threshold, double initTemp) 
    {
        if(threshold < 0 )
            throw edaException(this, "The threshold value is less than 0 !");
        if(threshold > initTemp)
            throw edaException(this, "The threshold value is less than initial temperate !");
        
        _threshold = threshold;
        _initTemp = initTemp;
    }
    
    virtual void set(double quantity) 
    {
        if( quantity <= 0 )
            throw edaException(this, "The quantity must be larger than 0 !");
        _quantity = quantity;  
    }
    
    virtual void set(double threshold, double quantity, double initTemp) 
    { 
        set(threshold, initTemp);
        set(quantity);
    }
    
    virtual void init()
    {
        checkError();
        _temp = _initTemp;
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) 
    {
        neighb.set(sol);
        neighb.init();
        
        edaNeighbour* Neighb = neighb.clone();        
        Neighb->rand();
        
        double gain = Neighb->evaluate();
        double prob = eda::rnd.uniform();
        double energy = exp( gain/_temp );  
        
        if( gain > 0 || prob < energy ) 
        {
            neighb = *Neighb;
        }     
        easerObject(Neighb);
    }    

    virtual void checkError() const 
    {
        if( eda::isNAN(_initTemp) )
            throw edaException(this, "The initial temperate not set !");
        if( eda::isNAN(_threshold) )
            throw edaException(this, "The threshold temperate not set !");
        if( eda::isNAN(_quantity) )
            throw edaException(this, "The changing ratio not set !");

    }
    
    virtual bool equil() {
        return (_temp -= _quantity) > _threshold;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDALINEARCOOLINGSCHEDULE_);      

    virtual const char* className() const 
    {
        return "edaLinearCoolingSchedule";
    }
    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack)
        {
            buf.pack(&_threshold);
            buf.pack(&_quantity);
            buf.pack(&_initTemp);
        }
        else
        {
            buf.unpack(&_threshold);
            buf.unpack(&_quantity);
            buf.unpack(&_initTemp); 
        }
    }
 
protected:
    double _threshold;
    double _quantity;
    double _initTemp;
    double _temp;

};

#endif	/* EDALINEARCOOLINGSCHEDULE_H */

