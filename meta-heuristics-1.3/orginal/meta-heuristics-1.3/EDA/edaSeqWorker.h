/* 
 * File:   edaSeqWorker.h
 * Author: Tieu Minh
 *
 * Created on April 11, 2014, 11:31 AM
 */

#ifndef EDASEQWORKER_H
#define	EDASEQWORKER_H

#include "edaObject.h"
#include "edaBuffer.h"
#include "edaSearch.h"


class edaSeqWorker : public edaObject
{
public:
    
    edaSeqWorker() :_search(NULL)
    {      
    }
    
    virtual ~edaSeqWorker() 
    {
        easerObject(_search);  
    }    
    
    virtual void set( edaBuffer &buf ) 
    {
        easerObject(_search);
        _search = (edaSearch*)unpack( buf );
    }
    
    virtual void search( edaBuffer& buf_pro, edaBuffer& buf_in, edaBuffer& buf_out ) 
    {
        // Create new problem object
        edaProblem *problem = (edaProblem*)unpack( buf_pro );
        
        // Create new solution object
        edaPopulation *pop = (edaPopulation*)unpack( buf_in );
        
        // Set the problem for population
        pop->reconfig(problem);
        
        // Call search algorithm
        _search->search( *pop );

        // pack output solution
        pop->pack( buf_out);

        // Clean up
        easerObject(pop);
        easerObject(problem);
    }
    
    virtual const char* className() const 
    {
        return "edaSeqWorker";
    }

protected:
    edaSearch *_search;    

};

#endif	/* EDASEQWORKER_H */

