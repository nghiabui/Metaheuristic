/* 
 * File:   tspGenne.h
 * Author: Tieu Minh
 *
 * Created on April 23, 2014, 9:37 AM
 */

#ifndef TSPGENNE_H
#define	TSPGENNE_H


#include "tspDefine.h"


class tspGenne : public edaGenne 
{
public:
    
    tspGenne(unsigned int value = eda::FLAG);
    
    ~tspGenne();
    
    const char* className() const;

    void print(ostream &os) const;
    
    tspGenne& operator = (const edaGenne &genne);
    
    tspGenne& operator = (unsigned int value);
    
    operator unsigned int();
    
    bool operator == (const edaGenne &genne) const;
    
    tspGenne * clone() const;
    
    void checkError() const;
    
protected:
    unsigned int _value;

};

#endif	/* TSPGENNE_H */

