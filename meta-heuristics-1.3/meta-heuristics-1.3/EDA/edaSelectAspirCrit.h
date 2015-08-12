/*
 * edaSelectionAspirCrit.h
 *
 *  Created on: May 6, 2015
 *      Author: nghia
 */

#ifndef META_HEURISTICS_1_3_EDA_EDASELECTIONASPIRCRIT_H_
#define META_HEURISTICS_1_3_EDA_EDASELECTIONASPIRCRIT_H_

#include "edaAspirCrit.h"
#include "edaGreedyStrategy.h"
#include "edaSelectImprovement.h"

class edaSelectAspirCrit: public edaAspirCrit {
public:
	edaSelectAspirCrit() :
			_gain(-eda::INF) {
		_stgy = new edaSelectImprovement();
	}

	edaSelectAspirCrit(const edaGreedyStrategy& stgy) :
			_stgy(NULL), _gain(-eda::INF) {
		set(stgy);
	}

	void set(const edaGreedyStrategy& stgy) {
		stgy.checkError();
		easerObject(_stgy);
		_stgy = stgy.clone();
	}

	virtual ~edaSelectAspirCrit() {
		easer();
	}

	virtual void init() {
		_gain = NAN;
	}

	virtual edaSelectAspirCrit* clone() const {
		edaSelectAspirCrit* aspir = new edaSelectAspirCrit();
		aspir->_gain = _gain;
		aspir->set(*_stgy);
		return aspir;
	}

	virtual void explore(edaNeighbour& neighb, const edaSolution& sol) {
		checkError();
		_stgy->explore(neighb, sol);
	}

	virtual void serialize(edaBuffer& buf, bool pack) {
		if (pack) {
			_stgy->pack(buf);
		} else {
			easer();
			_stgy = (edaGreedyStrategy*) unpack(buf);
		}
	}

	virtual const char* className() const {
		return "edaSelectAspirCrit";
	}

	virtual bool check(edaNeighbour& neighb) {
		double gain = neighb.evaluate();
		if (eda::isNAN(_gain)) { //firs time , initialize the variable
			_gain = gain;
			return true;
		} else {
			double currtardiness = (int) floor(_gain);
			double currmakespan = _gain - currtardiness;
			double tardiness = (int) floor(gain);
			double makespan = gain - tardiness;

//			if (_gain < gian)
			if((currtardiness<=tardiness)&&(currmakespan<=makespan))
			{

				return false;
			} else {
				_gain = gain;
				return true;
			}
		}
	}

	virtual void checkError() const {
		if (_stgy == NULL)
			throw edaException(this, "The strategy is not set !");
		if (_gain == -eda::INF)
			throw edaException(this, "The edaAspirCrit not initial !");
	}

	virtual void easer() {
		easerObject(_stgy);
	}

	setClassID(_SYSCLASSID_ + _CLSID_EDASELECTASPIRCRIT_)
	;

protected:
	edaGreedyStrategy* _stgy;
	double _gain;

};

#endif /* META_HEURISTICS_1_3_EDA_EDASELECTIONASPIRCRIT_H_ */
