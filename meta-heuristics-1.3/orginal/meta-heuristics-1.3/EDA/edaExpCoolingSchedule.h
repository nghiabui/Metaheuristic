/* 
 * File:   edaExpCoolingSchedule.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDAEXPCOOLINGSCHEDULE_H
#define	EDAEXPCOOLINGSCHEDULE_H

#include "edaCoolingSchedule.h"


class edaExpCoolingSchedule: public edaCoolingSchedule {
public:
    
    edaExpCoolingSchedule() : 
            _threshold(NAN), 
            _ratio(NAN), 
            _initTemp(NAN) 
    {}
    
    edaExpCoolingSchedule(double threshold, double initTemp, double ratio) 
    {
        set(threshold, initTemp, ratio);
    }        
            
    virtual ~edaExpCoolingSchedule() 
    {
    }    

    virtual edaExpCoolingSchedule* clone() const 
    {
        edaExpCoolingSchedule* cool = new edaExpCoolingSchedule();
        cool->_threshold = _threshold;
        cool->_initTemp = _initTemp;
        cool->_ratio = _ratio;
        cool->_temp = _temp;
        return cool;
    }
    
    virtual void set(double threshold, double initTemp) 
    {   
        if(threshold < 0 )
            throw edaException(this, "The threshold value is less than 0 !");
        if(threshold > initTemp)
            throw edaException(this, "The threshold value is less than initial temperate !");
        
        _threshold = threshold == 0 ? eda::epsilon : threshold;
        _initTemp = initTemp;      
    }
    
    virtual void set(double ratio) 
    {       
        if( ratio < 0 || ratio > 1 )
             throw edaException(this, "The ratio must be in [0, 1] !");
        
        _ratio = ratio;
    }
    
    virtual void set(double threshold, double initTemp, double ratio) 
    {
        set(threshold, initTemp);
        set(ratio);
    }
    
    virtual void init() 
    {
        checkError();
        _temp = _initTemp;
    }
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) 
    {  
        neighb.set(sol);
        
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
        if( eda::isNAN(_ratio) )
            throw edaException(this, "The changing ratio not set !");
    }
    
    virtual bool equil() 
    {
        return (_temp *= _ratio) > _threshold;
    }
    
    setClassID(_SYSCLASSID_ + _CLSID_EDAEXPCOOLINGSCHEDULE_);      


    virtual const char* className() const 
    {
        return "edaExpCoolingSchedule";
    }    
    
    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack)
        {
            buf.pack(&_threshold);
            buf.pack(&_ratio);
            buf.pack(&_initTemp);
        }
        else
        {
            buf.unpack(&_threshold);
            buf.unpack(&_ratio);
            buf.unpack(&_initTemp); 
        }
    }
 
protected:
    double _threshold;
    double _ratio;
    double _initTemp;
    double _temp;

};

#endif	/* EDAEXPCOOLINGSCHEDULE_H */

