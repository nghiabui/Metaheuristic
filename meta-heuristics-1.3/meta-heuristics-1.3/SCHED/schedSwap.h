/* 
 * File:   schedSwap.h
 * Author: Trung Nghia
 *
 * Created on March 19, 2014, 10:10 PM
 */

#ifndef schedSWAP_H
#define	schedSWAP_H

#include "schedDefine.h"
#include "schedNeighbour.h"
#include "schedFitness.h"
class schedSwap : public schedNeighbour {
public:    
    // Construction
    schedSwap();
    schedSwap(const schedSolution& route);
    ~schedSwap();
    
    // Method
    double evaluate();  
    schedSwap* clone() const;
    void update(edaSolution& sol);
    void next();
    void rand();
    void init();
    void print(ostream& os) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_schedSwap_);
    schedSwap& operator =(const edaNeighbour& neighb);
    bool operator ==(const edaNeighbour& neighb) const;
    const char* className() const;

private:
    unsigned int _first;
    unsigned int _second;
    double _gain;
};

#endif	/* schedSwap_H */

