/* 
 * File:   edaMoeBase.h
 * Author: Tieu Minh
 *
 * Created on April 9, 2014, 4:37 PM
 */

#ifndef EDAMOVEBASE_H
#define	EDAMOVEBASE_H

#include "edaSearch.h"


class edaMoveBase : public edaSearch
{
public:

    edaMoveBase() {}
    
    virtual ~edaMoveBase() {}
    
    virtual void search(edaPopulation& pop) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual void checkError() const = 0;

    virtual edaMoveBase* clone() const = 0;

    virtual const char* className() const = 0;
    
};

#endif	/* EDAMOVEBASE_H */

