/* 
 * File:   edaPopBase.h
 * Author: Tieu Minh
 *
 * Created on April 9, 2014, 4:38 PM
 */

#ifndef EDAPOPBASE_H
#define	EDAPOPBASE_H

#include "edaSearch.h"


class edaPopBase : public edaSearch
{
public:
    
    edaPopBase() {}
    
    virtual ~edaPopBase() {}
    
    virtual void search(edaPopulation& pop) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual void checkError() const = 0;

    virtual edaPopBase* clone() const = 0;

    virtual const char* className() const = 0;

};

#endif	/* EDAPOPBASE_H */

