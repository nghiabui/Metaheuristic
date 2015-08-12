/* 
 * File:   edaSearch.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 9:13 PM
 */

#ifndef EDASEARCH_H
#define	EDASEARCH_H

#include "edaSerialize.h"
#include "edaPopulation.h"


class edaSearch: public edaSerialize {
public:
    
    edaSearch() : TaskID(0), ProcID(0) {}
    
    virtual ~edaSearch() {}
    
    virtual void search(edaPopulation& pop) = 0;

    virtual void serialize(edaBuffer& buf, bool pack) 
    {
        if(pack)
        {
            buf.pack( &TaskID );
            buf.pack( &ProcID );
        }
        else
        {
            buf.unpack( &TaskID );
            buf.unpack( &ProcID );  
        }
    }

    virtual edaSearch* clone() const = 0;

    virtual const char* className() const = 0;
    
    virtual void checkError() const = 0;
    
//Properties
    unsigned int TaskID;
    unsigned int ProcID;
};

#endif	/* EDASEARCH_H */

