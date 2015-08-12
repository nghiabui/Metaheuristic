/* 
 * File:   edaCoolingSchedule.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 5:05 PM
 */

#ifndef EDACOOLINGSCHEDULE_H
#define	EDACOOLINGSCHEDULE_H

#include "edaStrategy.h"


class edaCoolingSchedule: public edaStrategy 
{
  public:

    virtual void set(double threshold, double initTemp) = 0;
    
    virtual void explore(edaNeighbour& neighb, const edaSolution& sol) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;
    
    virtual edaCoolingSchedule* clone() const = 0;
    
    virtual void init() = 0;
    
    virtual bool equil() = 0;    

    virtual void checkError() const = 0;

    virtual const char* className() const = 0;
    

};

#endif	/* EDACOOLINGSCHEDULE_H */

