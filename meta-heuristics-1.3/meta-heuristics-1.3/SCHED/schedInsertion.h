/* 
 * File:   schedInsertion.h
 * Author: Trung Nghia
 *
 * Created on April 15, 2014, 10:05 AM
 */

#ifndef schedINSERTION_H
#define	schedINSERTION_H

#include "schedDefine.h"
#include "schedNeighbour.h"

class schedInsertion : public schedNeighbour {
public:    
    // Construction
    schedInsertion(unsigned int lambda = 2);
    schedInsertion(const schedSolution& route, unsigned int lambda = 2);
    ~schedInsertion();
    
    // Method
    void set(unsigned int lambda);
    virtual double evaluate();
    void checkError() const;
    schedInsertion* clone() const;
    void update(edaSolution& sol);
    void next();
    void rand();
    void init();
    void print(ostream& os) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_schedINSERTION_);
    schedInsertion& operator =(const edaNeighbour& neighb);
    bool operator ==(const edaNeighbour& neighb) const;    
    const char* className() const;


public:
    unsigned int _from;
    unsigned int _to;
    unsigned int _lambda;
    double _gain;
};

#endif	/* schedINSERTION_H */

