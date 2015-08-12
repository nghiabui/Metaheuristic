/*
 * File:   edaSelectImprovement.h
 * Author: Tieu Minh
 *
 * Created on March 12, 2014, 6:55 AM
 */

#ifndef EDASELECTIMPROVEMENT_H
#define	EDASELECTIMPROVEMENT_H

#include "edaGreedyStrategy.h"
using namespace std;
#include <vector>

class edaSelectImprovement: public edaGreedyStrategy {
public:

	edaSelectImprovement() {
	}

	virtual ~edaSelectImprovement() {
	}

	virtual edaSelectImprovement* clone() const {
		return new edaSelectImprovement();
	}
	inline char is_equals(double A, double B) {

		static float maxRelativeError = 0.000001;
		static float maxAbsoluteError = 0.000001;
		if (fabs(A - B) < maxAbsoluteError)
			return 1;

		static float relativeError;
		if (fabs(B) > fabs(A))
			relativeError = fabs((A - B) / B);
		else
			relativeError = fabs((A - B) / A);
		if (relativeError <= maxRelativeError)
			return 1;

		return 0;
	}
	virtual void explore(edaNeighbour& neighb, const edaSolution& sol) {
		checkError();

		neighb.set(sol);
		neighb.init();

		vector<int> makespanvect;
		vector<int> tardinessvect;
		double best, gain;
		edaNeighbour *Neighb = neighb.clone();
		edaNeighbour* bestNeighb = neighb.clone();
		best = bestNeighb->evaluate();

//		PRINT_DEBUG
		do {
//			PRINT_DEBUG
			gain = neighb.evaluate();
			double tardiness = floor(gain);
			double makespan = gain - tardiness;

//			PRINT_DEBUG
//			printf("gain %10.20fl make span %10.20fl tardiness %10.20fl  \n ",gain, makespan, tardiness);
			if (makespanvect.size() > 0) {
				bool flag = true;
				for (unsigned int i = 0; i < makespanvect.size(); i++) {
					int mkspn = makespanvect.at(i);
					int tdnss = tardinessvect.at(i);
					if ((mkspn >= makespan) && (tdnss >= tardiness)) {
						makespanvect.erase(makespanvect.begin() + i);
						tardinessvect.erase(tardinessvect.begin() + i);
					}
					if ((mkspn >= makespan) && (tdnss >= tardiness)) {
						flag = false;
					}

				}
//				PRINT_DEBUG
				if (flag) {
					makespanvect.push_back(makespan);
					tardinessvect.push_back(tardiness);
				}
			} else {
				makespanvect.push_back(makespan);
				tardinessvect.push_back(tardiness);
			}
			if (gain > best) {
				best = gain;
				easerObject(bestNeighb);
				bestNeighb = neighb.clone();
			}
			neighb.next();
		} while (!(neighb == *Neighb));

		if (best > eda::epsilon)
			neighb = *bestNeighb;

		easerObject(Neighb);
		easerObject(bestNeighb);
	}

	virtual void checkError() const {
	}

	setClassID(_SYSCLASSID_ + _CLSID_EDASELECTIMPROVEMENT )
	;

	virtual void serialize(edaBuffer& buf, bool pack) {
	}

	virtual const char* className() const {
		return "edaSelectImprovement";
	}

};

#endif	/* edaSelectImprovement_H */

