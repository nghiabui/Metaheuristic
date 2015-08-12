/* 
 * File:   edaMpiWorkflow.h
 * Author: Tieu Minh
 *
 * Created on April 11, 2014, 11:33 PM
 */

#ifndef edaMpiWorkflow_H
#define	edaMpiWorkflow_H

#include "edaPopulation.h"
#include "edaWorkflow.h"

#include <list>
#include <mpi.h>
#include <vector>

// Status of workers in master
#define EDA_MPI_PROCESSOR_IDLE      0
#define EDA_MPI_PROCESSOR_COMPUTING 1

// Commands from master
#define EDA_MPI_START_SEARCH        0
#define EDA_MPI_QUIT                1

// Respond status from worker
#define EDA_MPI_WAITING_COMMAND     0
#define EDA_MPI_SEARCH_FINISHED     1
#define EDA_MPI_QUIT_DONE           2
#define EDA_MPI_UNKNOWN_COMMAND     3


typedef struct 
{
    unsigned int stat;
    unsigned int taskID;
} edaMpiProcStatus;

class edaMpiWorkflow : public edaWorkflow
{
public:

    edaMpiWorkflow() 
      : edaWorkflow()
    {  
    }
    
    edaMpiWorkflow(const edaProblem& pro) 
      : edaWorkflow( pro )
    {
    }          

    ~edaMpiWorkflow() 
    {
        easerArray(workers);
    }

    void search(edaPopulation &pop) 
    {             
        MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
        workers = new edaMpiProcStatus[nprocs];
        
        // variables
        int i, j, p, l;
        int command;
        int nidles;
        int idles[nprocs];
        MPI_Status mpiStat;

        // initialize status variables
        runningJobCount = 0;
        for( i = 1; i < nprocs; i++ )
        {
            workers[i].stat = EDA_MPI_PROCESSOR_IDLE;
            workers[i].taskID = -1;
        }

        // start the search
        while ( !allDone() ) 
        {
            vector<unsigned int> readyTasks = findReadyTask();
            vector<unsigned int>::iterator readyIter;

            // find idle processors
            nidles = 0;
            for( i = 1; i < nprocs; i++ ) 
            {
                if ( workers[i].stat == EDA_MPI_PROCESSOR_IDLE )
                idles[nidles++] = i;
            }
 

            // Create the search objects for all ready nodes
            p = 0;
            for (readyIter = readyTasks.begin(); readyIter != readyTasks.end(); readyIter++)
            {
                // task ID on DAG
                unsigned int taskID = *readyIter;

                // send computing request to first idle processor
                if ( p < nidles )
                {
                    // send command
                    command = EDA_MPI_START_SEARCH;
                    MPI_Send( &command, 1, MPI_INT, idles[p], 0, MPI_COMM_WORLD );
                    
                    // pack problem
                    edaBuffer pro_buf;
                    problem->pack( pro_buf );

                    // send buffer for search method
                    l = pro_buf.getActualSize();
                    MPI_Send( &l, 1, MPI_INT, idles[p], 0, MPI_COMM_WORLD );
                    MPI_Send( pro_buf.getBuffer(), pro_buf.getActualSize(), 
                            MPI_CHAR, idles[p], 0, MPI_COMM_WORLD );
                    
                    
                    // pack search and its parameters
                    edaBuffer sa_buf;
                    edaSearch *sa = taskDAG[taskID];
                    sa->ProcID = idles[p];
                    sa->pack( sa_buf );

                    // send buffer for search method
                    l = sa_buf.getActualSize();
                    MPI_Send( &l, 1, MPI_INT, idles[p], 0, MPI_COMM_WORLD );
                    MPI_Send( sa_buf.getBuffer(), sa_buf.getActualSize(), 
                            MPI_CHAR, idles[p], 0, MPI_COMM_WORLD );

                    // pack solution
                    edaBuffer pop_buf;
                    edaPopulation *Pop = chooseSolution( taskID, pop );
                    Pop->pack( pop_buf );

                    // send buffer for solution to worker
                    l = pop_buf.getActualSize();
                    MPI_Send( &l, 1, MPI_INT, idles[p], 0, MPI_COMM_WORLD );
                    MPI_Send( pop_buf.getBuffer(), l, MPI_CHAR, idles[p],
                            0, MPI_COMM_WORLD );

                    // set status of worker to computing
                    workers[idles[p]].stat = EDA_MPI_PROCESSOR_COMPUTING;
                    workers[idles[p]].taskID = taskID;
                    taskStatus[taskID] = STATUS_RUNNING;
                    runningJobCount ++;
                    p ++;
                } 
                else
                {
                    // no processor idle, then break the loop
                    break;
                }
            }

            unsigned int pollingResult = POLLING_NOCHANGE;
            unsigned int nodeFinished;

            // Polling for job finished
            while (true)
            {
                if ( runningJobCount == 0 )
                {
                    // Nothing to poll
                    pollingResult = POLLING_NOCHANGE;
                    break;
                }

                // poll for any return
                pollingResult = polling( nodeFinished );
                if ( pollingResult == POLLING_FINISHED )
                {
                    break;
                } 
                else if ( pollingResult == POLLING_ERROR )
                {
                    break;
                } 
                else 
                { 
                    // NOCHANGE
                }
            }

            // Job finished
            if ( pollingResult == POLLING_FINISHED )
            {
                taskStatus[workers[nodeFinished].taskID] = STATUS_FINISHED;
                runningJobCount--;

                // Get the solution
                edaBuffer result_buf;

                // receive the result status
                MPI_Recv( &l, 1, MPI_INT, nodeFinished, 0, MPI_COMM_WORLD, &mpiStat );
                if ( l == EDA_MPI_SEARCH_FINISHED ) 
                {
                    // receive the result into a buffer of characters
                    MPI_Recv( &j, 1, MPI_INT, nodeFinished, 0, MPI_COMM_WORLD, &mpiStat );
                    char *buf = new char[j];
                    MPI_Recv( buf, j, MPI_CHAR, nodeFinished, 0, MPI_COMM_WORLD, &mpiStat );
                    result_buf.setBuffer( j, buf );

                    edaPopulation *result = (edaPopulation*) unpack (result_buf);
                    result->reconfig(problem);
                    taskPop[workers[nodeFinished].taskID] = result;
                    searchOrder.push_back( workers[nodeFinished].taskID );
                    workers[nodeFinished].stat = EDA_MPI_PROCESSOR_IDLE;
                }
                checkLoopStatus(workers[nodeFinished].taskID);
            }

            // Job error
            if ( pollingResult == POLLING_ERROR )
            {
                taskStatus[workers[nodeFinished].taskID] = STATUS_ERROR;
                runningJobCount--;
                searchOrder.remove( workers[nodeFinished].taskID );
            }
        }
        
        unsigned int bestResultTaskID = detectBestResultTaskID();
        if ( !searchOrder.empty() && bestResultTaskID != eda::FLAG )
        {
            //Return the best result among results
            pop = *(taskPop[bestResultTaskID]);
        }
    }

    unsigned int polling( unsigned int &nodeID )
    {
        MPI_Status mpiStat;
        MPI_Probe( MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpiStat );
        nodeID = mpiStat.MPI_SOURCE;
        return POLLING_FINISHED;
    }
    

    virtual const char* className() const {
        return "edaMpiWorkflow";
    }


 private:
    std::list<unsigned int> searchOrder;
    edaMpiProcStatus *workers;
    unsigned int runningJobCount;
    int nprocs;
    
};

#endif	/* edaMpiWorkflow_H */

