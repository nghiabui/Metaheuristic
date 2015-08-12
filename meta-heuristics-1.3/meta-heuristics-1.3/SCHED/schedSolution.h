/* 
 * File:   schedSolution.h
 * Author: Trung Nghia
 *
 * Created on March 11, 2014, 10:10 PM
 */

#ifndef schedSOLUTION_H
#define	schedSOLUTION_H

#include "schedDefine.h"
#include "schedProblem.h"
#include "schedFitness.h"

class schedSolution: public edaSolution, public vector<unsigned int> {
public:
	//Construction
	schedSolution();
	schedSolution(const edaProblem& pro);
	~schedSolution();

	//Method
	void set(const edaProblem& pro);
	void set(const double fitness);
	bool load(const char* filename);
	const schedProblem& pro() const;
	void easer();
	schedSolution* clone() const;
	void print(ostream& os) const;
	void init();
	double evaluate();
	double getFitness() const;
	schedFitness* calFitness() const;
	void decode(const edaChromosome& chro);
	edaChromosome* encode() const;
	schedSolution& operator =(const edaSolution& Sol);
	bool operator ==(const edaSolution& sol) const;
	void serialize(edaBuffer& buf, bool pack);setClassID(_USERCLASSID_ + _CLSID_schedSOLUTION_)
	;
	void save(const char* filename);
	const char* className() const;
	void checkError() const;
	double convertSchedFitness(schedFitness* Fitness) const;
	int getCmax() const;
	void setCmax(int cmax);
	int getTardiness() const;
	void setTardiness(int tardiness);

private:
	void graphError() const;
	void initError() const;
	const schedProblem* _graph;
	double _fitness;
	int _Cmax;
	int _tardiness;
};

#endif	/* schedSOLUTION_H */

