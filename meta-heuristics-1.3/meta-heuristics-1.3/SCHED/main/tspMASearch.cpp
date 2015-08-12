/** 
 * File:   main.cpp
 * Author: Tieu Minh
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "tspProblem.h"
#include "tspSolution.h"
#include "tsp2Opt.h"
#include "tspInsertion.h"
#include "tspGenne.h"


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
        edaLoopCondition loop; 
        sched2Opt opt;
        
        edaHC hcSearch(opt, stgy, loop);
        
        edaTimeCondition crit(N);  
        edaRankSelection rank;        
        edaPMCrossover cross;
        edaRandSwapMutation mute;          
        cross.set( N / 20 );
        edaPopulation pop;
        pop.init(route, N);  
        #if COMM_MPI
    
            edaLoopCondition noLoop(0); 
            edaLoopCondition oneLoop(1); 


            edaHC trash(opt, stgy, noLoop);
            edaGA gaSearch(rank, cross, mute, oneLoop);         

            unsigned int trashid, hcid[N], gaid;

            trashid = workflow.insertVertex(trash);
            for(unsigned int i = 0; i < N; i++) 
            {
                edaIDCollect clect(i);
                hcid[i] = workflow.insertVertex (hcSearch, clect);                         
            }
            gaid = workflow.insertVertex (gaSearch); 

            for(unsigned int i = 0; i < N; i++) {
                workflow.insertEdge(trashid, hcid[i]);
                workflow.insertEdge(hcid[i], gaid);
            }

            workflow.insertLoop(gaid, trashid, crit);

            
        #else               
            edaRateAdapt adapt(hcSearch);
            edaMA ma(rank, cross, mute, adapt, crit); 
            workflow.insertVertex(ma);
     
        #endif

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
        case _USERCLASSID_ + _CLSID_TSPPROBLEM_:
            return new schedProblem();
            
        case _USERCLASSID_ + _CLSID_TSPSOLUTION_:
            return new schedSolution();
        
        case _USERCLASSID_ + _CLSID_TSP2OPT_:
            return new sched2Opt();    
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
