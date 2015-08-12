/* 
 * File:   tspSolution.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 10:10 PM
 */

#ifndef TSPSOLUTION_H
#define	TSPSOLUTION_H


#include "tspDefine.h"
#include "tspProblem.h"


class tspSolution: public edaSolution, public vector<unsigned int> {
public:
    //Construction
    tspSolution();
    tspSolution(const edaProblem& pro);
    ~tspSolution();
    
    //Method        
    void set(const edaProblem& pro);
    void set(const double fitness);
    bool load(const char* filename);
    const tspProblem& pro() const;
    void easer();
    tspSolution* clone() const;
    void print(ostream& os) const;
    void init();   
    double evaluate();
    void decode(const edaChromosome& chro);
    edaChromosome* encode() const;
    tspSolution& operator =(const edaSolution& Sol);
    bool operator == (const edaSolution& sol) const;
    void serialize(edaBuffer& buf, bool pack);
    setClassID(_USERCLASSID_ + _CLSID_TSPSOLUTION_);
    void save(const char* filename);  
    const char* className() const;
    void checkError() const;
    
private:
    void graphError() const;
    void initError() const;
    const tspProblem* _graph;
    double _fitness;
};

#endif	/* TSPSOLUTION_H */

