/* 
 * File:   edaExplorer.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 5:12 PM
 */

#ifndef EDAEXPLORER_H
#define	EDAEXPLORER_H

#include "edaOperator.h"
#include "edaNeighbour.h"
#include "edaStrategy.h"

class edaExplorer: public edaOperator 
{
public:
    
    edaExplorer() {}
    
    virtual ~edaExplorer() {}
    
    virtual void set(const edaStrategy& stgy) = 0;
    
    virtual void set(const edaNeighbour& neighb) = 0;
    
    virtual void init(const edaPopulation& pop) = 0;
    
    virtual edaExplorer* clone() const = 0;
    
    virtual bool update(edaPopulation& pop) = 0;        

    virtual void checkError() const = 0;

    virtual const char* className() const = 0;
    
};

#endif	/* EDAEXPLORER_H */

