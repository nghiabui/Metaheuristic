/* 
 * File:   edaDAGVertex.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 9:31 PM
 */


#ifndef EDADAGVERTEX_H
#define EDADAGVERTEX_H

#include <vector>
#include "edaDAGEdge.h"


class edaDAGEdge;

class edaDAGVertex : public edaObject
{
public:

    edaDAGVertex(unsigned int key): vertexKey (key) 
    {
    }
    
    virtual ~edaDAGVertex() 
    {
    }
    
    unsigned int getKey() const 
    {
        return vertexKey;    
    }
    
    void insertInEdge(edaDAGEdge *inEdge) 
    {
        inEdges.push_back(inEdge);    
    }
    
    void insertOutEdge(edaDAGEdge *outEdge) 
    {
        outEdges.push_back(outEdge);    
    }
    
    vector<edaDAGEdge *>& getInEdges() 
    {
        return inEdges;
    }
    
    vector<edaDAGEdge *>& getOutEdges() 
    {
        return outEdges;
    }
    
    bool cycleCheck(unsigned int &cycleVertex) 
    {
        if (cycleCheckFlag == true)
        {
          cycleVertex = vertexKey;
          return true;
        }

        cycleCheckFlag = true;

        for(unsigned int i = 0; i < outEdges.size(); i++ )
        {
            if (outEdges[i]->getDestVertex()->cycleCheck(cycleVertex) == true) 
            {
                return true;
            }
        }
        cycleCheckFlag = false;
        return false;    
    }
    
    unsigned int getInDegree() 
    {
        return inEdges.size();
    }    
    
    unsigned int getOutDegree() 
    {
        return outEdges.size();
    }
    
    const char* className() const 
    {
        return "edaDAGVertex";
    }

    void setCycleFlag(bool flag)
    {
        cycleCheckFlag = flag;
    }

    bool setCycleFlag() const
    {
        return cycleCheckFlag;
    }
    
private:
    vector<edaDAGEdge *> inEdges;
    vector<edaDAGEdge *> outEdges;
    bool cycleCheckFlag;
    unsigned int vertexKey;
};

#endif
