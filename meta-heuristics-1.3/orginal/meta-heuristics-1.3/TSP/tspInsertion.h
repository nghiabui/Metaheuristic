/* 
 * File:   tspInsertion.h
 * Author: Tieu Minh
 *
 * Created on April 15, 2014, 10:05 AM
 */

#ifndef TSPINSERTION_H
#define	TSPINSERTION_H

#include "tspDefine.h"
#include "tspNeighbour.h"

class tspInsertion : public tspNeighbour {
public:    
    // Construction
    tspInsertion(unsigned int lambda = 2);
    tspInsertion(const tspSolution& route, unsigned int lambda = 2);
    ~tspInsertion();
    
    // Method
    void set(unsigned int lambda);
    double evaluate();  
    void checkError() const;
    tspInsertion* clone() const;
    void update(edaSolution& sol);
    void next();
    void rand();
    void init();
    void print(ostream& os) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_TSPINSERTION_);
    tspInsertion& operator =(const edaNeighbour& neighb);
    bool operator ==(const edaNeighbour& neighb) const;    
    const char* className() const;


public:
    unsigned int _from;
    unsigned int _to;
    unsigned int _lambda;
    double _gain;
};

#endif	/* TSPINSERTION_H */

