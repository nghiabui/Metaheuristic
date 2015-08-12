/* 
 * File:   edaDAGEdge.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 9:31 PM
 */


#ifndef EDADAGEDGE_H
#define EDADAGEDGE_H

#include "edaObject.h"

class edaDAGVertex;

class edaDAGEdge : public edaObject
{
public:
    /**
     * Constructor
     */
    edaDAGEdge
    (
        unsigned int _key, 
        edaDAGVertex *_fromVertex,
        edaDAGVertex *_toVertex
    ):   
        fromVertex (_fromVertex), 
        toVertex (_toVertex), 
        edgeKey (_key) 
    {
    }
        
    virtual ~edaDAGEdge() 
    {
    }
    
    unsigned int getKey() const 
    {
        return edgeKey;        
    }
    
    edaDAGVertex* getSourceVertex() const 
    {
        return fromVertex;
    }
    

    edaDAGVertex* getDestVertex() const 
    {
        return toVertex;        
    }
    
    virtual const char* className() const 
    {
        return "edaDAGEdge";
    }
    
private:
    edaDAGVertex *fromVertex;
    edaDAGVertex *toVertex;
    unsigned int edgeKey;
};
#endif
