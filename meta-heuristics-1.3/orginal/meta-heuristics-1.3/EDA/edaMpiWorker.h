/* 
 * File:   edaMpiWorker.h
 * Author: Tieu Minh
 *
 * Created on April 12, 2014, 8:14 AM
 */

#ifndef EDAMPIWORKER_H
#define	EDAMPIWORKER_H

#include "edaMpiWorkflow.h"
#include <mpi.h>

class edaMpiWorker 
{
public:
    edaMpiWorker( int master = 0 ) 
        : master(master)
    {
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    }
    
    virtual ~edaMpiWorker()
    {        
    }

    void serve() 
    {
        // variables
        int command, l;
        int stat = EDA_MPI_WAITING_COMMAND;
        edaSearch *sa = NULL;
        edaPopulation *pop = NULL;
        edaProblem *pro = NULL;
        MPI_Status mpiStat;
        char *buf = NULL;

        while( stat == EDA_MPI_WAITING_COMMAND )
        {
            // wait command and print error if exists
            MPI_Recv( &command, 1, MPI_INT, MPI_ANY_SOURCE, 0, 
                    MPI_COMM_WORLD, &mpiStat );
            if ( mpiStat.MPI_SOURCE != master )
            {
                std::cerr << rank << ": command not from master " 
                        << master << std::endl; 
                continue;
            }

            // serve the command
            switch( command )
            {
                case EDA_MPI_START_SEARCH: 
                {
                    // get the problem
                    MPI_Recv( &l, 1, MPI_INT, master, 0, MPI_COMM_WORLD, &mpiStat );
                    buf = new char[l];
                    MPI_Recv( buf, l, MPI_CHAR, master, 0, MPI_COMM_WORLD, &mpiStat );
                    edaBuffer pro_buf;
                    pro_buf.setBuffer( l, buf );
                    pro = (edaProblem*)unpack( pro_buf );
                    
                    // get the search algorithm
                    MPI_Recv( &l, 1, MPI_INT, master, 0, MPI_COMM_WORLD, &mpiStat );
                    buf = new char[l];
                    MPI_Recv( buf, l, MPI_CHAR, master, 0, MPI_COMM_WORLD, &mpiStat );
                    edaBuffer sa_buf;
                    sa_buf.setBuffer( l, buf );
                    sa = (edaSearch*)unpack( sa_buf );

                    // get the solution
                    MPI_Recv( &l, 1, MPI_INT, master, 0, MPI_COMM_WORLD, &mpiStat );
                    buf = new char[l];
                    MPI_Recv( buf, l, MPI_CHAR, master, 0, MPI_COMM_WORLD, &mpiStat );
                    edaBuffer pop_buf;
                    pop_buf.setBuffer( l, buf );
                    pop = (edaPopulation*)unpack( pop_buf );                    
                    
                    // Reconfigure the problem for the solution
                    pop->reconfig(pro);
                    
                    // start the search
                    sa->search( *pop );

                    // write the status
                    stat = EDA_MPI_SEARCH_FINISHED;
                    break;
                }
                case EDA_MPI_QUIT :
                {
                    stat = EDA_MPI_QUIT_DONE;
                    break;
                }
                default :
                {
                    std::cerr << rank << ": unknown command from master " << master << std::endl;
                    stat = EDA_MPI_UNKNOWN_COMMAND;
                    break;
                }
            }

            // return the result if not quit
            if ( stat != EDA_MPI_QUIT_DONE )
                MPI_Send( &stat, 1, MPI_INT, master, 0, MPI_COMM_WORLD );
            else
                break;

            // and return additional result
            switch( stat )
            {
                case EDA_MPI_SEARCH_FINISHED :
                {
                    // return new solution to master
                    edaBuffer pop_buf;
                    pop->pack( pop_buf );
                    l = pop_buf.getActualSize();
                    MPI_Send( &l, 1, MPI_INT, master, 0, MPI_COMM_WORLD );
                    MPI_Send( pop_buf.getBuffer(), l, MPI_CHAR, master, 0, MPI_COMM_WORLD );

                    // delete search algorithm and solution
                    easerObject(sa);
                    easerObject(pop);
                    easerObject(pro);
                    break;
                }
                default :
                    break;
            }

            // update local variables
            stat = EDA_MPI_WAITING_COMMAND;
        }
    }
    
private:
    int master;
    int rank;
};

#endif	/* EDAMPIWORKER_H */

