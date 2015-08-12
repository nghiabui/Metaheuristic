/* 
 * File:   edaDefine.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 7:09 PM
 */

#ifndef EDADEFINE_H
#define	EDADEFINE_H


// Define the status code
#define STATUS_PENDING                        0
#define STATUS_READY                          1
#define STATUS_STARTING                       2
#define STATUS_RUNNING                        3
#define STATUS_FINISHED                       4
#define STATUS_ERROR                          5
#define STATUS_LOOPING                        6
#define STATUS_LOOP_READY                     7

// Define the polling results
#define POLLING_NOCHANGE                      0
#define POLLING_FINISHED                      1
#define POLLING_ERROR                         2


/* for Class Serialize */
#define _SYSCLASSID_                          50000
#define _USERCLASSID_                         0

/* for General Class */
#define _CLSID_EDAPOPULATION_                 1
        
#define _CLSID_EDALOOPCONDITION_              11
#define _CLSID_EDATIMECONDITION_              12
#define _CLSID_EDACONSTFITCONDITION_          13
#define _CLSID_EDAVARFITCONDITION_            14

#define _CLSID_EDABESTIMPROVEMENT_            50
#define _CLSID_EDAFIRSTIMPROVEMENT_           51
#define _CLSID_EDARANDIMPROVEMENT_            52
#define _CLSID_EDAEXPCOOLINGSCHEDULE_         53
#define _CLSID_EDALINEARCOOLINGSCHEDULE_      54
#define _CLSID_EDANOASPIRCRIT_                55
#define _CLSID_EDABESTASPIRCRIT_              56
#define _CLSID_EDARATEADAPT_                  57
#define _CLSID_EDASTATICTABU_                 58
#define _CLSID_EDARANKSELECTION_              59
#define _CLSID_EDAPMCROSSOVER_                60
#define _CLSID_EDARANDSWAPMUTATION_           61

#define _CLSID_EDAHC_                         20
#define _CLSID_EDASA_                         21
#define _CLSID_EDATS_                         22
#define _CLSID_EDAGA_                         23
#define _CLSID_EDAMA_                         24
#define _CLSID_EDAVNS_                        25

#define _CLSID_EDAHCEXPLORER_                 80
#define _CLSID_EDASAEXPLORER_                 81
#define _CLSID_EDATSEXPLORER_                 82
#define _CLSID_EDAVNSEXPLORER_                83


#define _CLSID_INVALID_                       99999


#include <fstream>
#include <iostream>
#include "edaRNG.h"
#include "edaTimer.h"
#include "edaMath.h"
#include "edaException.h"

using namespace std;

//Macro function
#define setClassID( id )                                        \
    virtual unsigned int getClassID() const {                   \
        return id;                                              \
    }
;

#ifdef _DEBUG
#define printDebug(dbglevel, dbg)                               \
    if (dbglevel <= _DEBUG && __logFlag != eda::FLAG) {         \
        cout << dbg << endl;                                    \
}
#else
#define printDebug(dbglevel, dbg)                           
#endif

#define enableLog()                                             \
    __logFlag = 1;                                          

#define disableLog()                                            \
    __logFlag = eda::FLAG;   

#define easerObject(object)                                     \
    if (object) {                                               \
        delete object;                                          \
        object = NULL;                                          \
    }
;

#define easerArray(array)                                       \
    if (array) {                                                \
        delete[] array;                                         \
        array = NULL;                                           \
    }
;

#define easerArrayObject(arrayObject, size)                     \
    if (arrayObject) {                                          \
        for (unsigned int i = 0; i < size; i++)                 \
            easerObject(arrayObject[i]);                        \
        delete[] arrayObject;                                   \
        arrayObject = NULL;                                     \
    }
;

#define easerVector(vector)                                     \
    for(unsigned int i = 0; i < vector.size(); i++)             \
    {                                                           \
        easerObject(vector[i]);                                 \
    }                                                           \
    vector.clear();


#define printCSVLog(task, proc, status, loop, pop)              \
    if(__logFlag == 0)                                          \
    {                                                           \
        if (__rank == 1 || __rank == 0)                         \
        {                                                       \
            __ofstr.open(__logName, ofstream::app);             \
            __ofstr << "Heuristic, Task, Proc, Status, BestID," \
                    " Time, NumLoop, Count, MaxEval,"           \
                    " MinEval, MeanEval, StdEval, BestSol"      \
                    << endl;                                    \
            __ofstr.close();                                    \
        }                                                       \
        __logFlag = 1;                                          \
    }                                                           \
    if(__logFlag == 1)                                          \
    {                                                           \
        unsigned int id = pop.best();                           \
        __ofstr.open(__logName, ofstream::app);                 \
        __ofstr << className() << ", "                          \
                << task << ", "                                 \
                << proc << ", "                                 \
                << status << ", "                               \
                << id << ", "                                   \
                << __timer.stop() << ", "                       \
                << loop << ", "                                 \
                << pop.size() << ", "                           \
                << pop.max() << ", "                            \
                << pop.min() << ", "                            \
                << pop.mean() << ", "                           \
                << pop.std() << ", ";                           \
        if( id == eda::FLAG )                                   \
            __ofstr << "Null" << endl;                          \
        else                                                    \
            __ofstr << *pop[id] << endl;                        \
        __ofstr.close();                                        \
    }
;


extern std::ofstream __ofstr;
extern edaTimer __timer;
extern char __logName[256]; 
extern char* __progname;
extern unsigned int __logFlag;
extern int __rank;
extern int __size;

namespace eda
{
    bool isNAN(double value);
    void swap(unsigned int &a, unsigned int &b);    
    static double epsilon = 1E-6;
    static double INF = 1E32;
    static edaRNG rnd;
    static edaMath math;  
    static unsigned int FLAG = -1;
}

inline bool eda::isNAN(double value) 
{
    if(value != value)
        return true;
    return false;
}

inline void eda::swap(unsigned int &a, unsigned int &b) 
{
    unsigned int temp = a;
    a = b;
    b = temp;
}


#ifdef COMM_MPI
#include <mpi.h>
#endif

#ifdef COMM_MPI
#define EDAMetasearchStart(argc, argv)                          \
    EDALogProcessStart()                                        \
    MPI_Init( &argc, &argv );                                   \
    MPI_Comm_rank( MPI_COMM_WORLD, &__rank );                   \
    MPI_Comm_size( MPI_COMM_WORLD, &__size );                   \
    if ( __size < 2 )                                           \
    {                                                           \
        cerr << "Number of processors must > 1" << endl;        \
        MPI_Finalize();                                         \
        return 0;                                               \
    }                                                           \
    try                                                         \
    {                                                           \
        if ( __rank == 0 )                                      \
        {
#else
#define EDAMetasearchStart(argc, argv)                          \
    EDALogProcessStart()                                        \
    try                                                         \
    {                                                           
#endif


#ifdef COMM_MPI
#define EDAMetasearchStop()                                     \
            int __c__ = EDA_MPI_QUIT;                           \
            for( int __p__ = 1; __p__<__size; __p__++ ){        \
                MPI_Send( &__c__, 1, MPI_INT,                   \
                        __p__, 0, MPI_COMM_WORLD );             \
            }                                                   \
        }                                                       \
        else                                                    \
        {                                                       \
            /* stuff listening to master requests here */       \
            eda::rnd.init(__rank);                              \
            edaMpiWorker w(0);                                  \
            w.serve();                                          \
        }                                                       \
        MPI_Finalize();                                         \
    }                                                           \
    catch (edaException& ex)                                    \
    {                                                           \
        cerr << ex.getErrorMessage();                           \
    };
#else
#define EDAMetasearchStop()                                     \
    }                                                           \
    catch (edaException& ex)                                    \
    {                                                           \
        cerr << ex.getErrorMessage();                           \
    };
#endif

#define EDALogProcessStart()                                    \
    __timer.start();                                            \
    __logFlag = 0;                                              \
    sprintf(__logName,"%s.csv", __progname);                    \
    remove (__logName);                                         \
;



#endif	/* EDADEFINE_H */

