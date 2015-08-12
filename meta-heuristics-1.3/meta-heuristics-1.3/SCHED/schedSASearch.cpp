/* 
 * File:   main.cpp
 * Author: Trung Nghia
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "schedProblem.h"
#include "schedSolution.h"
#include "sched2Opt.h"
#include "schedInsertion.h"


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
        edaExpCoolingSchedule sched(0, 50, 0.98);        
        sched2Opt opt;

        edaPopulation pop;
        pop.init(route, N);

        edaLoopCondition crit( 1500 );
        edaSA sa(opt, sched, crit);
        workflow.set(graph);
        workflow.insertVertex(sa);
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
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
