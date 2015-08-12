/* 
 * File:   schedProblem.h
 * Author: Trung Nghia
 *
 * Created on March 11, 2014, 8:31 PM
 */

#ifndef schedPROBLEM_H
#define	schedPROBLEM_H

#include "schedDefine.h"
#include "task.cpp"
#include "schedFitness.h"
class schedProblem: public edaProblem {
public:
    // Construction    
    schedProblem();  
    schedProblem(const char* filename);
    ~schedProblem();    
    
    // Method
    void load(const char* filename);    
    void easer();
    unsigned int size() const;    
    schedProblem* clone() const;
    int gettaskId(unsigned int id) const;
    int gettaskLength(unsigned int id) const;
    int gettaskDeadline(unsigned int id) const;
    void updatetaskTime(unsigned int id, unsigned int taskTime);

    schedFitness* calFitness(vector<unsigned int> vect);
 //   double distance(unsigned int from, unsigned int to) const;
    void serialize(edaBuffer& buf, bool pack);   
    setClassID(_USERCLASSID_ + _CLSID_schedPROBLEM_);
    void print(ostream& os) const;
    const char* className() const;
    void checkError() const;

	int getP() const {
		return _p;
	}

	void setP(int p) {
		_p = p;
	}

	int getT() const {
		return _T;
	}

	void setT(int t) {
		_T = t;
	}

	unsigned int getNumVert() const {
		return _numVert;
	}

	void setNumVert(unsigned int numVert) {
		_numVert = numVert;
	}

private:
    unsigned int _numVert;
    int *_vectCoord;
    char _filename[256];
    int _T;
    int _p;
};

#endif	/* schedPROBLEM_H */

