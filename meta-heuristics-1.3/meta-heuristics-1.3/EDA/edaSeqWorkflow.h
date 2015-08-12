/* 
 * File:   edaSeqWorkflow.h
 * Author: Tieu Minh
 *
 * Created on April 11, 2014, 11:33 PM
 */
#ifndef EDASEQWORKFLOW_H
#define EDASEQWORKFLOW_H

#include "edaWorkflow.h"
#include "edaSeqWorker.h"

class edaSeqWorkflow : public edaWorkflow
{
public:

    edaSeqWorkflow()  :
		edaWorkflow(), worker(NULL)
	{}

    edaSeqWorkflow(const edaProblem& pro) :
		edaWorkflow(pro), worker(NULL)
    {}

    virtual ~edaSeqWorkflow()
    {
    	easerObject(worker);
    }

    virtual void search(edaPopulation &pop);

    const char* className() const
    {
    	return "edaSeqWorkflow";
    }

private:
    edaSeqWorker *worker;

};

#endif  /* EDASEQWORKFLOW_H */
