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
        tspProblem graph(argv[1]); 
        tspSolution route(graph);
        edaBestAspirCrit aspir; 
        edaStaticTabu tabu(1200);
        tsp2Opt opt;

        edaPopulation pop;
        pop.init(route, N);

        edaLoopCondition crit( 1500 );
        edaTS ts(opt, aspir, tabu, crit);
        
        workflow.set(graph);
        workflow.insertVertex(ts);
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
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
