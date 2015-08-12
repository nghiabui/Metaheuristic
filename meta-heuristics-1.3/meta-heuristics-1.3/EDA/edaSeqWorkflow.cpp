#include "edaDefine.h"
#include "edaSearch.h"
#include "edaSerialize.h"

#include "edaSynthCollect.h"
#include "edaEliteCollect.h"
#include "edaIDCollect.h"

#include "edaMpiWorkflow.h"
#include "edaMpiWorker.h"
#include "edaSeqWorkflow.h"



void edaSeqWorkflow::search(edaPopulation &pop) {
	checkError();

	// pack problem
	edaBuffer pro_buf;
	problem->pack(pro_buf);

	// initialize worker
	worker = new edaSeqWorker();
	unsigned int lastSearch = eda::FLAG;

	// If all job is done, quit
	while (!allDone()) {
		// Find nodes that ready to run
		vector<unsigned int> readyNodes = findReadyTask();
		vector<unsigned int>::iterator intIter;

		for (intIter = readyNodes.begin(); intIter != readyNodes.end();
				intIter++) {
			edaSearch *sa = taskDAG[*intIter];
			sa->ProcID = 0;
			lastSearch = *intIter;
			edaPopulation *Pop = chooseSolution(*intIter, pop);
			// pack the search algorithm
			edaBuffer sa_buf;
			sa->pack(sa_buf);

			// pack the problem and solution
			edaBuffer pro_buf, pop_buf_in, pop_buf_out;
			problem->pack(pro_buf);
			Pop->pack(pop_buf_in);
			// Invoke wrapper's search method
			worker->set(sa_buf);
			worker->search(pro_buf, pop_buf_in, pop_buf_out);
			fflush (stdout);
			// Unpack and save the solution
			easerObject(taskPop[*intIter]);
			taskPop[*intIter] = (edaPopulation*) unpack(pop_buf_out);

			// Reconfigure the problem for the solution
			taskPop[*intIter]->reconfig(problem);

			taskStatus[*intIter] = STATUS_FINISHED;

			checkLoopStatus(*intIter);
			// Destroy objects
			easerObject(Pop);
		}
	}

	// Return the solution
	unsigned int bestResultTaskID = detectBestResultTaskID();
	if (lastSearch != eda::FLAG && bestResultTaskID != eda::FLAG) {
		//Return the best result among results
		pop = *(taskPop[bestResultTaskID]);
	}
}

