/* 
 * File:   edaObject.h
 * Author: Tieu Minh
 *
 * Created on April 8, 2014, 2:47 PM
 */

#ifndef EDAOBJECT_H
#define	EDAOBJECT_H

#include <iostream>
#include <stdlib.h>  


class edaObject {
    
public:
    
    edaObject() {}
    
    virtual ~edaObject() {}
    
    virtual const char * className() const  =  0;
    
    virtual void checkError() const {}
    
    virtual void print(std::ostream& os) const {}
};

inline std::ostream& operator << (std::ostream &os, const edaObject &obj) {
    obj.print(os);
    return os;
}
#endif	/* EDAOBJECT_H */

