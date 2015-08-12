/* 
 * File:   tspProblem.cpp
 * Author: Trung Nghia
 * 
 * Created on March 11, 2014, 8:31 PM
 */

#include "schedProblem.h"
#include <fstream>
#include "task.cpp"
#include "schedFitness.h"
schedProblem::schedProblem() {
	_numVert = 0;
	_vectCoord = NULL;
	strcpy(_filename, "");
}

schedProblem::schedProblem(const char* filename) {
	_vectCoord = NULL;
	load(filename);
}

schedProblem::~schedProblem() {
	easer();
}

unsigned int schedProblem::size() const {
	return _numVert;
}

int schedProblem::gettaskId(unsigned int id) const {
	return _vectCoord[id * 3];
}

int schedProblem::gettaskLength(unsigned int id) const {
	return _vectCoord[id * 3 + 1];
}
int schedProblem::gettaskDeadline(unsigned int id) const {
	return _vectCoord[id * 3 + 2];
}

void schedProblem::easer() {
	_numVert = 0;

	strcpy(_filename, "");

	if (_vectCoord != NULL)
		delete[] _vectCoord;

	_vectCoord = NULL;
}
schedFitness* schedProblem::calFitness(vector<unsigned int> vect) {
	schedFitness *result = new schedFitness();
	int Cmax = 0;

	int time = 0;
	int executed_periodic = 0;
	unsigned int i = 0;
	int j;
	do {
		int current_periodic = time / _T + 1;
		int task_Id = vect[i];
		if (current_periodic > executed_periodic) {
			executed_periodic++;
			time += _p;
//				 System.out.println("executed periodic task"+ Constants.p+
//				 "end at "+ time);

		} else {

			int tmp_time = (time + gettaskLength(task_Id));
			int tmp_periodic = tmp_time / _T + 1;
			if ((tmp_periodic > executed_periodic + 1)
					|| ((tmp_periodic == executed_periodic + 1)
							&& (tmp_time + _p) > ((executed_periodic + 1) * _T))) {
//					 System.out.println("time "+ time +"tmp_time "+ tmp_time+
//					 " executed_periodic "
//					 +executed_periodic+" current_periodic "+current_periodic+" tmp_periodic "+
//					 tmp_periodic +"(executed_periodic * Constants.T) "+
//					 (executed_periodic * Constants.T));
//				Task tmp_task = new Task();
//				tmp_task.setStart_time(time);
//				tmp_task.setStop_time((executed_periodic) * Constants.T);
//				tmp_task.setTask_type(TYPES.TASK_TYPE.IDLE);
//				tmp_task.settask_time((executed_periodic) * Constants.T
//						- time);
//
//				tmp_list.add(i, tmp_task);
				time = (executed_periodic) * _T;
//					 System.out.println("executed idle task"+
//					 tmp_task.gettask_time()+ "end at "+ time);

			} else {
//				Task tmp_task = tmp_list.get(i);
//				time += tmp_task.gettask_time();
				time += gettaskLength(task_Id);
				if (time > gettaskDeadline(task_Id))
					Cmax += time - gettaskDeadline(task_Id);
				i++;

			}
		}

	} while (i < _numVert);
	result->setTotalmakespan(time);
	result->setTotaltardiness(Cmax);
	return result;
}

void schedProblem::load(const char* filename) {
	// Clean old data
	easer();

	// Get file name
	strcpy(_filename, filename);

	// Read file
	ifstream file(_filename);
	if (file) {
		cout << ">> Loading [" << _filename << "]" << endl;
		file >> _p >> _T >> _numVert;
		cout << "p" << _p << " T" << _T << " num of task" << _numVert << "\n";
		_vectCoord = new int[_numVert * 4];
		cout << "reach this";
		for (unsigned int i = 0; i < _numVert; i++) {
			_vectCoord[i * 4] = i + 1;
		}
		for (unsigned int i = 0; i < _numVert; i++) {
			file >> _vectCoord[i * 4 + 1]; //task time
		}
		for (unsigned int i = 0; i < _numVert; i++) {
			file >> _vectCoord[i * 4 + 2]; //task deadline
		}

		file.close();

	} else {
		string error = "The file name '";
		error += filename;
		error += "' is not existing !";
		throw edaException(this, error);
	}
}
void schedProblem::updatetaskTime(unsigned int id, unsigned int tasktime) {
	_vectCoord[id * 4 + 3] = tasktime;
}

schedProblem* schedProblem::clone() const {
	schedProblem* result = new schedProblem();

	strcpy(result->_filename, _filename);

	result->_numVert = _numVert;
	result->_vectCoord = new int[this->_numVert * 4];
	result->_T = _T;
	result->_p = _p;

	memcpy(result->_vectCoord, _vectCoord, sizeof(int) * 4 * _numVert);
//	printf("schedProblem::clone _T %d clone->_T %d ", _T, result->_T);
	return result;
}

void schedProblem::print(ostream& os) const {
	checkError();
	os << ">> File [" << _filename << "]" << endl;
	os << "ID " << "\t" << "X " << "\t" << "Y " << endl;

	for (unsigned int i = 0; i < _numVert; i++) {
		os << i << "\t task id" << _vectCoord[i * 4] << "\t task lenght"
				<< _vectCoord[i * 4 + 1] << "\t task deadline "
				<< _vectCoord[i * 4 + 2] << "task time" << _vectCoord[i * 4 + 3]
				<< endl;
	}
}

void schedProblem::serialize(edaBuffer& buf, bool pack) {
	if (pack) {
		buf.pack(&_numVert);
		buf.pack(&_T);
		buf.pack(&_p);
		buf.pack(_vectCoord, 4 * _numVert);
		buf.pack(_filename, 256);
	} else {
		easer();
		buf.unpack(&_numVert);
		buf.unpack(&_T);
		buf.unpack(&_p);

		_vectCoord = new int[4 * _numVert];
		buf.unpack(_vectCoord, 4 * _numVert);
		buf.unpack(_filename, 256);
	}
}

const char* schedProblem::className() const {
	return "schedProblem";
}

void schedProblem::checkError() const {
	if (strcmp(_filename, "") == 0)
		throw edaException(this, "Not load input problem file !");
	if (_numVert == 0)
		throw edaException(this, "The input problem doesn't have the stops !");
}
