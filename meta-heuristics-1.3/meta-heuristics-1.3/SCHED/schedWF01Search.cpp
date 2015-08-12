/*  * File:   main.cpp
 * Author: Trung Nghia
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "schedProblem.h"
#include "schedSolution.h"
#include "schedInsertion.h"
#include "sched2Opt.h"
#include "schedSwap.h"

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
        
        edaBestImprovement stgy;
        edaExpCoolingSchedule sched(0, 50, 0.98);   
        edaSelectAspirCrit aspir1,aspir2,aspir3,aspir4;
        edaStaticTabu tabu1(120),tabu2(100),tabu3(100),tabu4(100);
        
        edaRankSelection rank;        
        edaPMCrossover cross;
        edaRandSwapMutation mute;
        
        sched2Opt opt1,opt2;
        schedSwap swap1,swap2;
        edaPopulation pop;
        pop.init(route, N);
        edaLoopCondition crit1( 150 ),crit2( 100 ),crit3( 150 ),crit4( 50 );
        
        edaTS tsSearch1(opt1, aspir1, tabu1, crit1);
        edaTS tsSearch2(opt2, aspir2, tabu2, crit2);//
        edaTS tsSearch3(swap1, aspir3, tabu3, crit3);
        edaTS tsSearch4(swap2, aspir4, tabu4, crit4);

        unsigned int tsid1 = workflow.insertVertex (tsSearch1);
        unsigned int tsid2 = workflow.insertVertex (tsSearch2);
        unsigned int tsid3 = workflow.insertVertex (tsSearch3);
        unsigned int tsid4 = workflow.insertVertex (tsSearch4);
        
        workflow.insertEdge(tsid1, tsid2);
        workflow.insertEdge(tsid1, tsid3);
        workflow.insertEdge(tsid2, tsid4);
        workflow.insertEdge(tsid3, tsid4);

        workflow.set(graph);
        workflow.search(pop);  
        
		fflush(stdout);
		for(int i=0;i<pop.size();i++) {
			schedSolution& result = *(schedSolution*) pop[i];
			result.evaluate();
			int Cmax=result.getCmax();
			cout << "[Route]  " << result << endl;
			schedFitness *Fitness= result.calFitness();
			cout << "[Fitness] Cmax " << Fitness->getTotalmakespan() <<" Tardiness "<<Fitness->getTotaltardiness() << endl;
								fflush(stdout);
		}

		fflush(stdout);
        
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
        case _USERCLASSID_ + _CLSID_schedSwap_:
                    return new schedSwap();
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
