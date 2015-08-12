/* 
 * File:   tspProblem.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 8:31 PM
 */

#ifndef TSPPROBLEM_H
#define	TSPPROBLEM_H

#include "tspDefine.h"


class tspProblem: public edaProblem {
public:
    // Construction    
    tspProblem();  
    tspProblem(const char* filename);
    ~tspProblem();    
    
    // Method
    void load(const char* filename);    
    void easer();
    unsigned int size() const;    
    tspProblem* clone() const;
    double x(unsigned int id) const;
    double y(unsigned int id) const;
    double distance(unsigned int from, unsigned int to) const;
    void serialize(edaBuffer& buf, bool pack);   
    setClassID(_USERCLASSID_ + _CLSID_TSPPROBLEM_);
    void print(ostream& os) const;
    const char* className() const;
    void checkError() const;

    
private:
    unsigned int _numVert;
    double *_vectCoord;
    char _filename[256];
};

#endif	/* TSPPROBLEM_H */

