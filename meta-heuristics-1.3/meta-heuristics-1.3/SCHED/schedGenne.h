/* 
 * File:   schedGenne.h
 * Author: Trung Nghia
 *
 * Created on April 23, 2014, 9:37 AM
 */

#ifndef schedGENNE_H
#define	schedGENNE_H


#include "schedDefine.h"


class schedGenne : public edaGenne 
{
public:
    
    schedGenne(unsigned int value = eda::FLAG);
    
    ~schedGenne();
    
    const char* className() const;

    void print(ostream &os) const;
    
    schedGenne& operator = (const edaGenne &genne);
    
    schedGenne& operator = (unsigned int value);
    
    operator unsigned int();
    
    bool operator == (const edaGenne &genne) const;
    
    schedGenne * clone() const;
    
    void checkError() const;
    
protected:
    unsigned int _value;

};

#endif	/* schedGENNE_H */

