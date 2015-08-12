/*
 * edaFitness.h
 *
 *  Created on: May 20, 2015
 *      Author: nghia
 */

#ifndef META_HEURISTICS_1_3_EDA_EDAFITNESS_H_
#define META_HEURISTICS_1_3_EDA_EDAFITNESS_H_
class edaFitness: public edaObject {

public:
	edaFitness() {
		_Cmax = 0;
		_Tardiness = 0;
	}

	~edaFitness() {
	}

	const char * className() const {
		return "edaFitness";
	}

	void checkError() const {
	}

	void print(std::ostream& os) const {
		printf("_Cmax %d _Tardiness %d \r\n", _Cmax, _Tardiness);
	}

	int getCmax() const {
		return _Cmax;
	}

	void setCmax(int cmax) {
		_Cmax = cmax;
	}

	int getTardiness() const {
		return _Tardiness;
	}

	void setTardiness(int tardiness) {
		_Tardiness = tardiness;
	}

protected:
	int _Cmax;
	int _Tardiness;
}
;

#endif /* META_HEURISTICS_1_3_EDA_EDAFITNESS_H_ */
