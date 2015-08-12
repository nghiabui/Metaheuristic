/* 
 * File:   edaOperator.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:39 AM
 */

#ifndef EDAOPERATOR_H
#define	EDAOPERATOR_H

#include "edaPopulation.h"


class edaOperator: public edaSerialize 
{
public:

    edaOperator() {}
    
    virtual ~edaOperator() {}
    
    virtual bool update(edaPopulation& pop) = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaOperator* clone() const = 0;

};

#endif	/* EDAOPERATOR_H */

