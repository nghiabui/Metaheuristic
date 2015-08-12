/* 
 * File:   edaProblem.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 6:57 PM
 */

#ifndef EDAPROBLEM_H
#define	EDAPROBLEM_H

#include "edaSerialize.h"


class edaProblem : public edaSerialize {
public:
    
    edaProblem() {}
    
    virtual ~edaProblem() {}
    
    virtual edaProblem* clone() const = 0;
    
    virtual void load(const char* filename) = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;

    virtual const char* className() const  = 0;
    
};

#endif	/* EDAPROBLEM_H */

