/* 
 * File:   edaGenne.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 10:02 PM
 */

#ifndef EDAGENNE_H
#define	EDAGENNE_H

#include "edaObject.h"
#include "edaDefine.h"


class edaGenne : public edaObject {
public:    
    
    edaGenne() {}
    
    virtual ~edaGenne() {}
    
    static void swap(edaGenne& A, edaGenne& B)
    {
        edaGenne* C = A.clone();
        A = B;
        B = *C;
        easerObject(C);
    }  
    static void swap(edaGenne* A, edaGenne* B)
    {
        return swap(*A, *B);
    } 
    
    virtual const char* className() const = 0;

    virtual void print(ostream &os) const = 0;   
    
    virtual edaGenne& operator = (const edaGenne &genne) = 0;  
    
    virtual bool operator == (const edaGenne &genne) const = 0;   
    
    bool operator != (const edaGenne &genne) const
    {
        return ! operator ==(genne);
    } 

    virtual edaGenne* clone() const = 0;
    
    virtual void checkError() const = 0;

};

#endif	/* EDAGENNE_H */

