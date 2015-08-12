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
        unsigned int N = 20;
        schedProblem graph(argv[1]); 
        schedSolution route(graph);   
        
        edaExpCoolingSchedule sched(0, 50, 0.98);   
                
        sched2Opt opt;
        edaPopulation pop;
        pop.init(route, 1);
        edaLoopCondition crit( 1500 );   

        edaRankSelection rank;        
        edaPMCrossover cross;
        edaRandSwapMutation mute;
        
        edaSA saSearch(opt, sched, crit);
        edaGA gaSearch(rank, cross, mute, crit); 
        
        unsigned int said[N], gaid;
        
        for(unsigned int i = 0; i < N; i++) 
        {
            said[i] = workflow.insertVertex (saSearch);        
        }
        gaid = workflow.insertVertex (gaSearch);   
        
        for(unsigned int i = 0; i < N; i++) 
        {
            workflow.insertEdge(said[i], gaid);
        }
        
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
