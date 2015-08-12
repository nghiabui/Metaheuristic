/*
 * schedFitness.h
 *
 *  Created on: Apr 15, 2015
 *      Author: nghiabui
 */

#ifndef SCHED_SCHEDFITNESS_H_
#define SCHED_SCHEDFITNESS_H_
#include <stdio.h>
class schedFitness {
public:
	schedFitness();
	virtual ~schedFitness();

	int getTotalmakespan() const {
		return totalmakespan;
	}

	void setTotalmakespan(int totalmakespan) {
		this->totalmakespan = totalmakespan;
	}

	int getTotaltardiness() const {
		return totaltardiness;
	}

	void setTotaltardiness(int totaltardiness) {
		this->totaltardiness = totaltardiness;
	}
	void print(){
		printf("totalmakespan %d totaltardiness %d \n",totalmakespan,totaltardiness);
	}

private:
	int totaltardiness;
	int totalmakespan;
};
#endif /* SCHED_SCHEDFITNESS_H_ */
