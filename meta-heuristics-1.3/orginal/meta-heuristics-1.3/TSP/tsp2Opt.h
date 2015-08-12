/* 
 * File:   tsp2Opt.h
 * Author: Tieu Minh
 *
 * Created on March 19, 2014, 10:10 PM
 */

#ifndef TSP2OPT_H
#define	TSP2OPT_H

#include "tspDefine.h"
#include "tspNeighbour.h"

class tsp2Opt : public tspNeighbour {
public:    
    // Construction
    tsp2Opt();
    tsp2Opt(const tspSolution& route);
    ~tsp2Opt();
    
    // Method
    double evaluate();  
    tsp2Opt* clone() const;
    void update(edaSolution& sol);
    void next();
    void rand();
    void init();
    void print(ostream& os) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_TSP2OPT_);
    tsp2Opt& operator =(const edaNeighbour& neighb);
    bool operator ==(const edaNeighbour& neighb) const;
    const char* className() const;


private:
    unsigned int _first;
    unsigned int _second;
    double _gain;
};

#endif	/* TSP2OPT_H */

