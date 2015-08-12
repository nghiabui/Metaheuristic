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
        unsigned int N = 40;
        schedProblem graph(argv[1]); 
        schedSolution route(graph);   
        
        edaBestImprovement stgy;
        edaExpCoolingSchedule sched(0, 50, 0.98);   
        edaBestAspirCrit aspir; 
        edaStaticTabu tabu(1200);
        
        
        sched2Opt opt;
        edaPopulation pop;
        pop.init(route, N);
        edaLoopCondition crit( 1500 );   
        edaLoopCondition loop(1);   
        edaEliteCollect elite;
        
        edaSA saSearch(opt, sched, crit);
        edaTS tsSearch(opt, aspir, tabu, crit);
        edaHC hcSearch(opt, stgy, crit);
        
        
        unsigned int tsid0 = workflow.insertVertex (tsSearch);
        unsigned int said1 = workflow.insertVertex (saSearch);   
        unsigned int said2 = workflow.insertVertex (saSearch);  
        unsigned int said3 = workflow.insertVertex (saSearch);  
        unsigned int hcid4 = workflow.insertVertex (hcSearch, elite);
       
        workflow.insertEdge(tsid0, said1);
        workflow.insertEdge(tsid0, said2);
        workflow.insertEdge(tsid0, said3);
        workflow.insertEdge(said1, hcid4);
        workflow.insertEdge(said2, hcid4);
        workflow.insertEdge(said3, hcid4);
        workflow.insertLoop(hcid4, tsid0, loop);
        
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
