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

    edaSeqWorkflow() 
      : edaWorkflow(), worker(NULL)
    {
    }
    
    edaSeqWorkflow(const edaProblem& pro) 
      : edaWorkflow( pro ), worker(NULL)
    {
    }

    virtual ~edaSeqWorkflow() 
    {       
        easerObject(worker);
    }
    
    void search(edaPopulation &pop) 
    {
        checkError();
        
        // pack problem
        edaBuffer pro_buf;
        problem->pack( pro_buf );
        
        // initialize worker
        worker = new edaSeqWorker();  
        unsigned int lastSearch = eda::FLAG; 
        
        // If all job is done, quit
        while (!allDone())
        {
            // Find nodes that ready to run
            vector<unsigned int> readyNodes = findReadyTask();
            vector<unsigned int>::iterator intIter;

            for (intIter = readyNodes.begin(); intIter != readyNodes.end(); intIter++)
            {
                edaSearch *sa = taskDAG[*intIter];
                sa->ProcID = 0;
                lastSearch = *intIter;
                edaPopulation *Pop = chooseSolution( *intIter, pop );

                // pack the search algorithm
                edaBuffer sa_buf;
                sa->pack( sa_buf );

                // pack the problem and solution
                edaBuffer pro_buf, pop_buf_in, pop_buf_out;
                problem->pack( pro_buf );
                Pop->pack( pop_buf_in );

                // Invoke wrapper's search method
                worker->set(sa_buf);
                worker->search( pro_buf, pop_buf_in, pop_buf_out );

                // Unpack and save the solution
                easerObject(taskPop[*intIter]);
                taskPop[*intIter] = (edaPopulation*)unpack( pop_buf_out );
                
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
        if(lastSearch != eda::FLAG && bestResultTaskID != eda::FLAG)
        {
            //Return the best result among results
            pop = *(taskPop[bestResultTaskID]);
        }
    }  

    const char* className() const 
    {
        return "edaSeqWorkflow";
    }

private:
    edaSeqWorker *worker;

};

#endif  /* EDASEQWORKFLOW_H */
