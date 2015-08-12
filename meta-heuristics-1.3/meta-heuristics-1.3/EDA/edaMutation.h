/* 
 * File:   edaMutation.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 3:46 PM
 */

#ifndef EDAMUTATION_H
#define	EDAMUTATION_H

#include "edaOperator.h"


class edaMutation : public edaOperator 
{
public:
    
    virtual bool update(edaPopulation& pop) = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaMutation* clone() const = 0;

};

#endif	/* EDAMUTATION_H */

