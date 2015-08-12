/* 
 * File:   main.cpp
 * Author: Trung Nghia
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "schedProblem.h"
#include "schedSolution.h"
#include "schedInsertion.h"
#include "sched2Opt.h"


#define N graph.size()

using namespace std;    


#if COMM_MPI
    static edaMpiWorkflow workflow;
#else
    static edaSeqWorkflow workflow;
#endif
    
int main(int argc, char** argv)
{    
    EDAMetasearchStart(argc, argv);
    if (argc != 2)
    {
      std::cerr << "Usage : ./" <<  __progname
                << " [instance]" << std::endl;
    }
    else
    {
        schedProblem graph(argv[1]); 
        schedSolution route(graph);  
        edaBestImprovement stgy;
         
        edaVarFitCondition recur;
        
        edaPopulation pop;
        pop.init(route, N);
        
        //Number neighbors 
        unsigned int n = 5;        
        unsigned int id[n];           
        
        edaLoopCondition crit( 1500 );          
        for(unsigned int i = 0; i < n; i++)
        {
            schedInsertion neighb( i + 2 );        
            
            edaHC hc(neighb, stgy, crit);
            
            id[i] = workflow.insertVertex(hc);
            
            if( i > 0)
            {
                workflow.insertEdge(id[i-1], id[i]);           
            }
        }        
        workflow.insertLoop(id[n-1], id[0], recur);
        workflow.set(graph);
        workflow.search(pop);    
        
        schedSolution& result = *(schedSolution*) pop[pop.best()];
        cout << "[Route] " << result << endl;
        cout << "[Fitness] " << result.evaluate () << endl;        
    }
    EDAMetasearchStop();
    return 0;
}
edaSerialize* userClassGenerate (unsigned int clsid)
{
    switch (clsid)
    {
        case _USERCLASSID_ + _CLSID_schedPROBLEM_:
            return new schedProblem();
            
        case _USERCLASSID_ + _CLSID_schedSOLUTION_:
            return new schedSolution();
        
        case _USERCLASSID_ + _CLSID_sched2OPT_:
            return new sched2Opt();    

        case _USERCLASSID_ + _CLSID_schedINSERTION_:
            return new schedInsertion();    
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
