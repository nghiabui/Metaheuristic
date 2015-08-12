#include "edaDefine.h"
#include "edaSearch.h"
#include "edaSerialize.h"

#include "edaSynthCollect.h"
#include "edaEliteCollect.h"
#include "edaIDCollect.h"

#include "edaMpiWorkflow.h"
#include "edaMpiWorker.h"
#include "edaSeqWorkflow.h"
#include "edaSeqWorker.h"

#include "edaLoopCondition.h"
#include "edaTimeCondition.h"
#include "edaConstFitCondition.h"
#include "edaVarFitCondition.h"

#include "edaBestImprovement.h"
#include "edaFirstImprovement.h"
#include "edaRandImprovement.h"
#include "edaSelectImprovement.h"

#include "edaExpCoolingSchedule.h"
#include "edaLinearCoolingSchedule.h"

#include "edaNoAspirCrit.h"
#include "edaBestAspirCrit.h"
#include "edaStaticTabu.h"
#include "edaSelectAspirCrit.h"

#include "edaRankSelection.h"
#include "edaRandSwapMutation.h"
#include "edaPMCrossover.h"
#include "edaRateAdapt.h"

#include "edaSAExplorer.h"
#include "edaHCExplorer.h"
#include "edaTSExplorer.h"

#include "edaHC.h"
#include "edaSA.h"
#include "edaTS.h"
#include "edaGA.h"
#include "edaMA.h"

int __size, __rank;
std::ofstream __ofstr;
char __logName[256];
edaTimer __timer;
unsigned __logFlag;

#pragma weak userClassGenerate
edaSerialize* userClassGenerate(unsigned int clsid) {
	cerr << "User must override userClassGenerate() undefined class id" <<clsid<< endl;
	exit(-1);
}
edaSerialize* classGenerate(unsigned int clsid) {
	switch (clsid) {
	case (_SYSCLASSID_ + _CLSID_EDAPOPULATION_):
		return (edaSerialize *) new edaPopulation();

	case (_SYSCLASSID_ + _CLSID_EDALOOPCONDITION_):
		return (edaSerialize *) new edaLoopCondition();

	case (_SYSCLASSID_ + _CLSID_EDATIMECONDITION_):
		return (edaSerialize *) new edaTimeCondition();

	case (_SYSCLASSID_ + _CLSID_EDACONSTFITCONDITION_):
		return (edaSerialize *) new edaConstFitCondition();

	case (_SYSCLASSID_ + _CLSID_EDAVARFITCONDITION_):
		return (edaSerialize *) new edaVarFitCondition();

	case (_SYSCLASSID_ + _CLSID_EDAHC_):
		return (edaSerialize *) new edaHC();

	case (_SYSCLASSID_ + _CLSID_EDABESTIMPROVEMENT_):
		return (edaSerialize *) new edaBestImprovement();

	case (_SYSCLASSID_ + _CLSID_EDAFIRSTIMPROVEMENT_):
		return (edaSerialize *) new edaFirstImprovement();

	case (_SYSCLASSID_ + _CLSID_EDARANDIMPROVEMENT_):
		return (edaSerialize *) new edaRandImprovement();
	case (_SYSCLASSID_ + _CLSID_EDASELECTASPIRCRIT_):
		return (edaSerialize *) new edaSelectAspirCrit();
	case (_SYSCLASSID_ + _CLSID_EDASELECTIMPROVEMENT):
		return (edaSerialize *) new edaSelectImprovement();
	case (_SYSCLASSID_ + _CLSID_EDAHCEXPLORER_):
		return (edaSerialize *) new edaHCExplorer();

	case (_SYSCLASSID_ + _CLSID_EDASA_):
		return (edaSerialize *) new edaSA();

	case (_SYSCLASSID_ + _CLSID_EDAEXPCOOLINGSCHEDULE_):
		return (edaSerialize *) new edaExpCoolingSchedule();

	case (_SYSCLASSID_ + _CLSID_EDALINEARCOOLINGSCHEDULE_):
		return (edaSerialize *) new edaLinearCoolingSchedule();

	case (_SYSCLASSID_ + _CLSID_EDASAEXPLORER_):
		return (edaSerialize *) new edaSAExplorer();

	case (_SYSCLASSID_ + _CLSID_EDATS_):
		return (edaSerialize *) new edaTS();

	case (_SYSCLASSID_ + _CLSID_EDASTATICTABU_):
		return (edaSerialize *) new edaStaticTabu();

	case (_SYSCLASSID_ + _CLSID_EDABESTASPIRCRIT_):
		return (edaSerialize *) new edaBestAspirCrit();

	case (_SYSCLASSID_ + _CLSID_EDANOASPIRCRIT_):
		return (edaSerialize *) new edaNoAspirCrit();

	case (_SYSCLASSID_ + _CLSID_EDATSEXPLORER_):
		return (edaSerialize *) new edaTSExplorer();

	case (_SYSCLASSID_ + _CLSID_EDAMA_):
		return (edaSerialize *) new edaMA();

	case (_SYSCLASSID_ + _CLSID_EDARATEADAPT_):
		return (edaSerialize *) new edaRateAdapt();

	case (_SYSCLASSID_ + _CLSID_EDAGA_):
		return (edaSerialize *) new edaGA();

	case (_SYSCLASSID_ + _CLSID_EDARANKSELECTION_):
		return (edaSerialize *) new edaRankSelection();

	case (_SYSCLASSID_ + _CLSID_EDARANDSWAPMUTATION_):
		return (edaSerialize *) new edaRandSwapMutation();

	case (_SYSCLASSID_ + _CLSID_EDAPMCROSSOVER_):
		return (edaSerialize *) new edaPMCrossover();

	default:
		return userClassGenerate(clsid);
	}
}
