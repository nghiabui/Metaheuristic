/*  * File:   main.cpp
 * Author: Tieu Minh
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "tspProblem.h"
#include "tspSolution.h"
#include "tspInsertion.h"
#include "tsp2Opt.h"


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
        tspProblem graph(argv[1]); 
        tspSolution route(graph);   
        
        edaBestImprovement stgy;
        edaExpCoolingSchedule sched(0, 50, 0.98);   
        edaBestAspirCrit aspir; 
        edaStaticTabu tabu(1200);
        
        edaRankSelection rank;        
        edaPMCrossover cross;
        edaRandSwapMutation mute;
        
        tsp2Opt opt;
        edaPopulation pop;
        pop.init(route, N);
        edaLoopCondition crit( 1500 );    
        
        edaSA saSearch(opt, sched, crit);
        edaTS tsSearch(opt, aspir, tabu, crit);
        edaHC hcSearch(opt, stgy, crit);
        edaGA gaSearch(rank, cross, mute, crit); 
        
        unsigned int said0 = workflow.insertVertex (saSearch);   
        unsigned int said1 = workflow.insertVertex (saSearch);  
        unsigned int tsid2 = workflow.insertVertex (tsSearch);
        unsigned int tsid3 = workflow.insertVertex (tsSearch);
        unsigned int hcid4 = workflow.insertVertex (hcSearch);
        unsigned int hcid5 = workflow.insertVertex (hcSearch);
        unsigned int gaid6 = workflow.insertVertex (gaSearch);
       
        workflow.insertEdge(said0, hcid4);
        workflow.insertEdge(hcid4, tsid2);
        workflow.insertEdge(said1, hcid5);
        workflow.insertEdge(hcid5, tsid3);
        workflow.insertEdge(tsid3, gaid6);
        workflow.insertEdge(tsid2, gaid6);
        
        workflow.set(graph);
        workflow.search(pop);  
        
        tspSolution& result = *(tspSolution*) pop[pop.best()];
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
        case _USERCLASSID_ + _CLSID_TSPPROBLEM_:
            return new tspProblem();
            
        case _USERCLASSID_ + _CLSID_TSPSOLUTION_:
            return new tspSolution();
        
        case _USERCLASSID_ + _CLSID_TSP2OPT_:
            return new tsp2Opt();    

        case _USERCLASSID_ + _CLSID_TSPINSERTION_:
            return new tspInsertion();    
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
