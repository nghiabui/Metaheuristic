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
         
        edaVarFitCondition recur;
        
        edaPopulation pop;
        pop.init(route, N);
        
        //Number neighbors 
        unsigned int n = N/10;        
        unsigned int id[n];           
        
        edaTimeCondition crit( 1.0* N/n );          
        for(unsigned int i = 0; i < n; i++)
        {
            tspInsertion neighb( i + 2 );        
            
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
