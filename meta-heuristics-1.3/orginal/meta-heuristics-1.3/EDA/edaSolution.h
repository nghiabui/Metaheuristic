/* 
 * File:   edaSolution.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 6:56 PM
 */

#ifndef EDASOLUTION_H
#define	EDASOLUTION_H

#include "edaProblem.h"
#include "edaChromosome.h"


class edaSolution: public edaSerialize {
public:
    
    edaSolution() {}
    
    virtual ~edaSolution() {}
    
    virtual edaSolution* clone() const = 0;
    
    virtual void set(const edaProblem& pro) = 0;
    
    virtual void set(const double fintess) = 0;
    
    virtual const edaProblem& pro() const = 0;
    
    virtual double evaluate() = 0;
    
    virtual void init() = 0;         

    virtual void print(ostream& os) const = 0;
    
    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaSolution& operator = (const edaSolution& sol) = 0;
    
    virtual bool operator == (const edaSolution& sol) const = 0;

    virtual const char* className() const = 0;
    
    virtual edaChromosome* encode() const 
    {
        return NULL;
    }
    
    virtual void decode(const edaChromosome& chro) {}
    
};

inline bool __comparator(edaSolution* A, edaSolution* B) {
    return A->evaluate() > B->evaluate();
}

#endif	/* EDASOLUTION_H */

