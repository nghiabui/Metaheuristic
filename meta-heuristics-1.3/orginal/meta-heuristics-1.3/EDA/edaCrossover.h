/* 
 * File:   edaCrossover.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 3:44 PM
 */

#ifndef EDACROSSOVER_H
#define	EDACROSSOVER_H

#include "edaOperator.h"


class edaCrossover : public edaOperator 
{
public:
    
    virtual bool update(edaPopulation& pop) = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaCrossover* clone() const = 0;

};

#endif	/* EDACROSSOVER_H */

