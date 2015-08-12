/* 
 * File:   sched2Opt.h
 * Author: Trung Nghia
 *
 * Created on March 19, 2014, 10:10 PM
 */

#ifndef sched2OPT_H
#define	sched2OPT_H

#include "schedDefine.h"
#include "schedNeighbour.h"
#include "schedFitness.h"
class sched2Opt : public schedNeighbour {
public:    
    // Construction
    sched2Opt();
    sched2Opt(const schedSolution& route);
    ~sched2Opt();
    
    // Method
    double evaluate();  
    sched2Opt* clone() const;
    void update(edaSolution& sol);
    void next();
    void rand();
    void init();
    void print(ostream& os) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_sched2OPT_);
    sched2Opt& operator =(const edaNeighbour& neighb);
    bool operator ==(const edaNeighbour& neighb) const;
    const char* className() const;

private:
    unsigned int _first;
    unsigned int _second;
    double _gain;
};

#endif	/* sched2OPT_H */

