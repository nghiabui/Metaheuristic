/* 
 * File:   main.cpp
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
        
        edaExpCoolingSchedule sched(0, 50, 0.98);   
                
        tsp2Opt opt;
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
