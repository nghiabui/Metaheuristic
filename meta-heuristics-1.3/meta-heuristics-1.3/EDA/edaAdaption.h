/* 
 * File:   edaAdaption.h
 * Author: Tieu Minh
 *
 * Created on April 18, 2014, 12:33 PM
 */

#ifndef EDAADAPTION_H
#define	EDAADAPTION_H

#include "edaSearch.h"
#include "edaOperator.h"
#include "edaMoveBase.h"


class edaAdaption : public edaOperator 
{
public:
    
    virtual void set(const edaMoveBase& localSearch) = 0;   

    virtual bool update(edaPopulation& pop) = 0;

    virtual void checkError() const = 0;

    virtual edaAdaption* clone() const = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;
    
};

#endif	/* EDAADAPTION_H */

