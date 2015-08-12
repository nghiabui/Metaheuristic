/* 
 * File:   schedSolution.cpp
 * Author: Trung Nghia
 * 
 * Created on March 11, 2014, 10:10 PM
 */

#include "schedSolution.h"
#include "schedGenne.h"
#include <iterator>
#include <algorithm>

schedSolution::schedSolution() {
	_fitness = NAN;
	_graph = NULL;
}

schedSolution::~schedSolution() {
	easer();
}

void schedSolution::set(const edaProblem& pro) {
	schedProblem& graph = (schedProblem&) pro;
	_graph = &graph;
}
double schedSolution::convertSchedFitness(schedFitness* Fitness) const {

	int makespan = Fitness->getTotalmakespan();
	int tardiness = Fitness->getTotaltardiness();
	double result = makespan;
	while (result > 1)
		result = result / 10;
	result += tardiness;

	return result;

}
double schedSolution::evaluate() {
	checkError();
	schedFitness* a = calFitness();
	return convertSchedFitness(a);

}
double schedSolution::getFitness() const {
//	PRINT_DEBUG
	schedFitness* a = calFitness();
	return convertSchedFitness(a);
}

schedFitness* schedSolution::calFitness() const {
	schedFitness *result = new schedFitness();
	int tardiness = 0;

	int time = 0;
	int executed_periodic = 0;
	unsigned int i = 0;
	int j;
	do {
		int current_periodic = time / _graph->getT() + 1;
		int task_Id = (*this)[i];
//		printf("time %d T %d task_Id %d \n",time,_graph->getT(),task_Id);
		if (current_periodic > executed_periodic) {
			executed_periodic++;
			time += _graph->getP();
//			PRINT_DEBUG
//			printf("current_periodic %d executed_periodic %d setTotalmakespan %d  setTotaltardiness %d \n",current_periodic, executed_periodic,time,Cmax);
		} else {

			int tmp_time = (time + _graph->gettaskLength(task_Id));
			int tmp_periodic = tmp_time / _graph->getT() + 1;
			if ((tmp_periodic > executed_periodic + 1)
					|| ((tmp_periodic == executed_periodic + 1)
							&& (tmp_time + _graph->getP())
									> ((executed_periodic + 1) * _graph->getT()))) {

				time = (executed_periodic) * _graph->getT();
//				PRINT_DEBUG
//				printf("setTotalmakespan %d  setTotaltardiness %d \n",time,Cmax);

			} else {

				time += _graph->gettaskLength(task_Id);
				if (time > _graph->gettaskDeadline(task_Id))
					tardiness += time - _graph->gettaskDeadline(task_Id);
				i++;
//				PRINT_DEBUG
//				printf("setTotalmakespan %d  setTotaltardiness %d \n",time,tardiness);

			}
		}

	} while (i < _graph->getNumVert());
//	PRINT_DEBUG
//	printf("setTotalmakespan %d  setTotaltardiness %d \n",time,Cmax);
	result->setTotalmakespan(time);
	result->setTotaltardiness(tardiness);
	return result;
}

void schedSolution::init() {
	graphError();

	clear();
	unsigned int size = _graph->size();
	for (unsigned int i = 0; i < size; i++) {
		push_back(i);
	}
	// Swap cities
	for (unsigned int i = 0; i < size; i++) {
		unsigned int j = eda::rnd.random(size);
		eda::swap(at(i), at(j));
//		printf("swap at %d and %d\n",i,j);
	}
	// Set NAN vale for route length
	set (NAN);
}

schedSolution* schedSolution::clone() const {
//	printf(" schedSolution clone   %s:%d: \n", __FILE__, __LINE__);
	schedSolution* route = new schedSolution();
	route->vector<unsigned int>::operator =(*this);
	route->_graph = _graph;
	route->_fitness = _fitness;
//	printf(" end ofschedSolution clone   %s:%d: \n", __FILE__, __LINE__);
	return route;
}

void schedSolution::print(ostream& os) const {
	checkError();
	copy(begin(), end(), ostream_iterator<int>(os, " "));
}

void schedSolution::set(const double fitness) {
	_fitness = fitness;
}

void schedSolution::easer() {
	_fitness = NAN;
	_graph = NULL;
	clear();
}

const schedProblem& schedSolution::pro() const {
	graphError();

	return *_graph;
}

schedSolution& schedSolution::operator =(const edaSolution& sol) {
	const schedSolution& route = (schedSolution&) sol;
	vector<unsigned int>::operator =(route);
	_fitness = route._fitness;
	_graph = route._graph;
	return *this;
}

schedSolution::schedSolution(const edaProblem& pro) {
	_fitness = NAN;
	set(pro);
}

void schedSolution::save(const char* filename) {
	checkError();

//    ofstream file;
//    file.open(filename);
//
//    unsigned int size = this->size();
//    for(unsigned int i = 0; i < size; i++)
//        file << _graph->x(at(i)) << " ";
//    file << endl;
//
//    for(unsigned int i = 0; i < size; i++)
//        file << _graph->y(at(i)) << " ";
//    file.close();
}

void schedSolution::serialize(edaBuffer& buf, bool pack) {
	unsigned int n = size();
	unsigned int value;
	if (pack) {
		buf.pack(&n);
		for (unsigned int i = 0; i < n; i++) {
			value = at(i);
			buf.pack(&value);
		}
		buf.pack(&_fitness);

	} else {
		easer();
		buf.unpack(&n);
		for (unsigned int i = 0; i < n; i++) {
			buf.unpack(&value);
			push_back(value);
		}
		buf.unpack(&_fitness);
	}
}

bool schedSolution::load(const char* filename) {
	graphError();

	ifstream file(filename);
	if (file) {
		clear();
		unsigned int size = _graph->size();
		resize(size);
		for (unsigned int i = 0; i < size; i++) {
			file >> at(i);
		}
		file.close();
		set (NAN);
	} else {
		string error = "The file name '";
		error += filename;
		error += "' is not existing !";
		throw edaException(this, error);
	}
	return false;
}

bool schedSolution::operator ==(const edaSolution& sol) const {
	const schedSolution& route = (schedSolution&) sol;
	return equal(begin(), end(), route.begin());
}

const char* schedSolution::className() const {
	return "schedSolution";
}

void schedSolution::checkError() const {
	graphError();

	initError();
}

void schedSolution::graphError() const {
	if (_graph == NULL)
		throw edaException(this, "Not set graph !");
	_graph->checkError();
}

void schedSolution::initError() const {
	if (size() == 0)
		throw edaException(this, "Not initial the route !");
	if (size() != _graph->size())
		throw edaException(this, "The route not satisfy the graph !");
}

void schedSolution::decode(const edaChromosome& chro) {
	unsigned int size = chro.length();
	for (unsigned int i = 0; i < size; i++) {
		schedGenne* genne = (schedGenne*) chro.at(i);
		at(i) = *genne;
	}
	_fitness = NAN;
}

edaChromosome* schedSolution::encode() const {
	edaChromosome* chro = new edaChromosome();
	unsigned int size = this->size();
	for (unsigned int i = 0; i < size; i++) {
		schedGenne genne = at(i);
		chro->add(genne);
	}
	return chro;
}
int schedSolution::getCmax() const {
	return _Cmax;
}
void schedSolution::setCmax(int cmax) {
	_Cmax = cmax;
}
int schedSolution::getTardiness() const {
	return _tardiness;
}
void schedSolution::setTardiness(int tardiness) {
	_tardiness = tardiness;
}

