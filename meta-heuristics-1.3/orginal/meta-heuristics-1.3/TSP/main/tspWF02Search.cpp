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
        tspProblem graph(argv[1]); 
        tspSolution route(graph);   
        
        edaBestImprovement stgy;
        edaExpCoolingSchedule sched(0, N, 0.98);   
        edaBestAspirCrit aspir; 
        edaStaticTabu tabu(N);
                
        tsp2Opt opt;
        edaPopulation pop;
        pop.init(route, 1);
        edaTimeCondition crit( N/60. );   
        edaLoopCondition noLoop(0);  
        
        edaSA saSearch(opt, sched, crit);
        edaTS tsSearch(opt, aspir, tabu, crit);
        edaHC hcSearch(opt, stgy, crit);
        edaHC trash(opt, stgy, noLoop);
        
        unsigned int hcid[N], said[N], tsid[N], trashid;
        
        for(unsigned int i = 0; i < N; i++) 
        {
            said[i] = workflow.insertVertex (saSearch);      
            tsid[i] = workflow.insertVertex (tsSearch);
            hcid[i] = workflow.insertVertex (hcSearch);         
        }
        trashid = workflow.insertVertex (trash);   
        
        for(unsigned int i = 0; i < N; i++) 
        {
            workflow.insertEdge(said[i], tsid[i]);
            workflow.insertEdge(tsid[i], hcid[i]);
            workflow.insertEdge(hcid[i], trashid);
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
