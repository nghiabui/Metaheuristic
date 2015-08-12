/* 
 * File:   edaDAG.h
 * Author: Tieu Minh
 *
 * Created on March 16, 2014, 9:31 PM
 */


#ifndef EDADAG_H
#define EDADAG_H

#include "edaDAGVertex.h"
#include "edaDAGEdge.h"
#include "edaException.h"
#include "edaCluster.h"
#include "edaDefine.h"
#include <map>


using namespace std;

template <class DataType>
class edaDAG : public edaObject
{
  public:
    edaDAG() 
    {
    }

    ~edaDAG()
    {
        map<unsigned int, edaDAGVertex *>::iterator vit;
        map<unsigned int, edaDAGEdge *>::iterator eit;
        map<unsigned int, edaDAGEdge *>::iterator lit;
        typename map<unsigned int, DataType>::iterator dit;
        
        for( vit=vertices.begin(); vit!=vertices.end(); vit++ )
            easerObject(vit->second);
        for( eit=edges.begin(); eit!=edges.end(); eit++ )
            easerObject(eit->second);
        for( lit=loops.begin(); lit!=loops.end(); lit++) 
            easerObject(lit->second);
        for( dit=data.begin(); dit!=data.end(); dit++) 
            easerObject(dit->second);
    }

    unsigned int count() const
    {
        return data.size();
    }

    void insertVertex(unsigned int key, DataType &_data) 
    {
        // Check for duplicate vertex
        if (vertices.find(key) != vertices.end())
        {
            throw edaException(this, "Vertex duplicated !");
        }

        // Create new vertex
        edaDAGVertex *vertex = new edaDAGVertex(key);

        // insert to vertices list
        vertices[key] = vertex;

        // and insert to data list
        data[key] = _data;
    }

    void insertEdge(unsigned int key, unsigned int fromVertex, unsigned int toVertex)
    {
        // Check for duplicate edge
        if (edges.find(key) != edges.end())
        {
            throw edaException(this, "Edge duplicated !");
        }

        // Source vertex not found
        if (vertices.find(fromVertex) == vertices.end())
        {
            throw edaException(this, "Source vertex not found !");
        }

        // Destination vertex not found
        if (vertices.find(toVertex) == vertices.end())
        {
            throw edaException(this ,"Destination vertex not found !");
        }
        
        // Insert new edge
        edaDAGEdge *edge = new edaDAGEdge(key, vertices[fromVertex], vertices[toVertex]);
        edges[key] = edge;
        
        // Update inbound edge and outbound edge of vertex
        vertices[fromVertex]->insertOutEdge(edge);
        vertices[toVertex]->insertInEdge(edge);
        
        // Check cycle
        unsigned int vertex;
        if( cycleCheck(vertex) )
        {
            char str[4];
            sprintf(str, "%d", vertex);
            string error = "Has a cycle in vertex ";  
            error += str;
            error += " !";
            throw edaException( this, error.data() );    
        }
    }

    bool cycleCheck(unsigned int &cycleVertex)
    {
        map<unsigned int, edaDAGVertex *>::iterator mapIterator;
        
        // Unset the flag of vertices
        for (mapIterator = vertices.begin(); mapIterator != vertices.end(); mapIterator++)
        {
            (*mapIterator).second->setCycleFlag(false);
        }

        // Run the cycle check
        for (mapIterator = vertices.begin(); mapIterator != vertices.end(); mapIterator++)
        {
            if ((*mapIterator).second->cycleCheck(cycleVertex))
            {
                // There is a cycle
                return true;
            }
        }
        return false;
    }

    edaDAGVertex* getVertex(unsigned int key)
    {
        map<unsigned int, edaDAGVertex *>::iterator mapIterator;
        mapIterator = vertices.find(key);

        // Not found
        if (mapIterator == vertices.end())
        {
            return NULL;
        }
        return (*mapIterator).second;
    }

    edaDAGEdge* getEdge(unsigned int key)
    {
        map<unsigned int, edaDAGEdge *>::iterator mapIterator;
        mapIterator = edges.find(key);

        // Not found
        if (mapIterator == edges.end())
        {
            return NULL;
        }
        return (*mapIterator).second;
    }

    DataType& operator [](unsigned int key) 
    {
        map<unsigned int, edaDAGVertex *>::iterator mapIterator;
        mapIterator = vertices.find(key);

        // Not found
        if (mapIterator == vertices.end())
        {
            throw new edaException(this, "Vertex not found !");
        }
        return data[key];
    }

    vector<unsigned int> getParentNodes(unsigned int key) const
    {
        edaDAGVertex *vertex;
        vector<edaDAGEdge *> inEdges;
        vector<unsigned int> parentNodes;
        map<unsigned int, edaDAGVertex *>::const_iterator mci;

        mci = vertices.find(key);
        vertex = mci->second;
        inEdges = vertex->getInEdges();

        vector<edaDAGEdge *>::const_iterator edgeIter;
        for (edgeIter = inEdges.begin(); edgeIter != inEdges.end(); edgeIter++)
        {
            parentNodes.push_back(((*edgeIter)->getSourceVertex())->getKey());
        }
        return parentNodes;
    }

    vector<unsigned int> traverse() const
    {
        vector<unsigned int> nodeList;
        typename map<unsigned int, DataType>::const_iterator iter;

        for (iter = data.begin(); iter != data.end(); iter++)
        {
            nodeList.push_back(iter->first);
        }

        return nodeList;
    }

    const char* className() const 
    {
        return "edaDAG";
    }
    
    void insertLoop(unsigned int key, unsigned int fromVertex, unsigned int toVertex) 
    {
        // Check for simple loop
        bool checkInvert = clusterCheck(fromVertex, toVertex);     
        if (clusterCheck(toVertex, fromVertex) || checkInvert ) 
        {
            if(checkInvert) 
            {       
                unsigned int tmp = fromVertex;
                fromVertex = toVertex;
                toVertex = tmp;
            }        
            if(! loops.empty()) 
            {
                edaCluster *newClus = createCluster(fromVertex, toVertex);
                map<unsigned int, edaDAGEdge*>::iterator lit;
                for(lit = loops.begin(); lit != loops.end(); lit++) 
                {
                    edaCluster *clus = createCluster(lit->second->getSourceVertex()->getKey(),
                            lit->second->getDestVertex()->getKey() );            
                    //Overlap between cluster and sub cluster
                    if(!newClus->isCover(clus) && !clus->isCover(newClus) && newClus->isOverlap(clus) ) 
                        throw edaException( this, "The insert loop overlap with the others !" );
                    //Identical
                    if(newClus->isCover(clus) && clus->isCover(newClus) ) 
                        throw edaException( this, "The insert loop duplicated !" );
                    easerObject(clus);
                } 
                easerObject(newClus);
            }    
            edaDAGEdge *loop = new edaDAGEdge(key, vertices[fromVertex], vertices[toVertex]);
            loops[key] = loop;
        }
        else 
        {
            throw edaException( this, "The insert loop not create a cluster !" );
        }
    }
    
    edaDAGEdge* findLoop(unsigned int taskID, map<unsigned int, unsigned int>& loopStatus) 
    {
        map<unsigned int, edaDAGEdge *>::iterator mapIterator;
        
        edaDAGEdge* result = NULL;        
        vector<edaDAGEdge *> stack;
        
        for(mapIterator = loops.begin(); mapIterator != loops.end(); mapIterator++)
        {     
            edaDAGEdge* loop = mapIterator->second;
            if( loopStatus[loop->getKey()] != STATUS_FINISHED &&
                    loop->getSourceVertex()->getKey() == taskID ) 
                stack.push_back(loop);
        }
        if(stack.size() == 1 ) 
        {
            result = stack[0];            
        }
        else if (stack.size() > 1)
        {
            unsigned int min = 0;
            edaCluster *minCluster = createCluster(stack[min]);
            for(unsigned int i = 1; i < stack.size(); i++)
            {   
                edaCluster *cluser = createCluster(stack[i]);
                if( minCluster->isCover(cluser) ) 
                {
                    easerObject(minCluster);
                    min = i;
                    minCluster = createCluster(stack[min]);
                }
                easerObject(cluser);
            }
            easerObject(minCluster);
            result = stack[min];
        }
        stack.clear();
        return result;
    }
    
    edaDAGEdge* detectLoop(unsigned int taskID, map<unsigned int, unsigned int>& loopStatus) 
    {
        map<unsigned int, edaDAGEdge *>::iterator mapIterator;
        
        edaDAGEdge* result = NULL;        
        vector<edaDAGEdge *> stack;
        
        for(mapIterator = loops.begin(); mapIterator != loops.end(); mapIterator++)
        {     
            edaDAGEdge* loop = mapIterator->second;
            if( loopStatus[loop->getKey()] != STATUS_FINISHED &&
                    loop->getDestVertex()->getKey() == taskID ) 
                stack.push_back(loop);
        }
        if(stack.size() == 1 ) 
        {
            result = stack[0];            
        }
        else if (stack.size() > 1)
        {
            unsigned int min = 0;
            edaCluster *minCluster = createCluster(stack[min]);
            for(unsigned int i = 1; i < stack.size(); i++)
            {   
                edaCluster *cluser = createCluster(stack[i]);
                if( minCluster->isCover(cluser) ) 
                {
                    easerObject(minCluster);
                    min = i;
                    minCluster = createCluster(stack[min]);
                }
                easerObject(cluser);
            }
            easerObject(minCluster);
            result = stack[min];
        }
        stack.clear();
        return result;
    }
    
    edaCluster* createCluster(const unsigned int &fromVertex,const unsigned int &toVertex) 
    {             
        map<unsigned int, edaDAGVertex *> loop;
        edaCluster *cluster = new edaCluster();      
        vector<edaDAGVertex *> stack;
        
        stack.push_back(vertices[toVertex]);
        do 
        { 
            edaDAGVertex *vertex = stack.back();
            stack.pop_back();            
            if(vertex->getKey() != fromVertex) 
            {
                vector<edaDAGEdge *> edgeList = vertex->getOutEdges();
                for(unsigned int i = 0; i < edgeList.size(); i++)     
                {  
                    edaDAGVertex* nextVertex = edgeList[i]->getDestVertex();
                    stack.push_back(nextVertex);
                }     
            }
            loop[vertex->getKey()] = vertex;
        } while (!stack.empty());

        map<unsigned int, edaDAGVertex *>::iterator it;
        for(it = loop.begin(); it != loop.end(); it++ ) 
        {
            cluster->push_back(it->second);
        }
        stack.clear();
        return cluster;
    }
    
    edaCluster* createCluster(const edaDAGEdge* loop) 
    {
        unsigned int fromVertex = loop->getSourceVertex()->getKey();
        unsigned int toVertex = loop->getDestVertex()->getKey();
        return createCluster(fromVertex, toVertex);    
    }
    
    vector<edaDAGEdge*> detectSubLoop(const edaDAGEdge& loop) 
    {
        vector<edaDAGEdge*> result; 
        edaCluster* base = createCluster(&loop);
        map<unsigned int, edaDAGEdge *>::iterator it;
        for(it = loops.begin(); it != loops.end(); it++ ) 
        {
            edaCluster* sub = createCluster(it->second);
            bool isSub = base->isCover(sub);
            if( isSub && !sub->isCover(base) ) 
            {
                result.push_back(it->second);
            }
            easerObject(sub);
        }
        easerObject(base);
        return result;
    }
    
    bool clusterCheck(const unsigned int &fromVertex,const unsigned int &toVertex) 
    {
        vector<edaDAGVertex *> stack;    

        //Checking left to right
        stack.push_back(vertices[fromVertex]);
        do 
        { 
            edaDAGVertex *vertex = stack.back();
            unsigned int key = vertex->getKey(); 
            stack.pop_back();            

            //The final vertex must be toVertex
            if(vertex->getOutDegree() == 0 && key != toVertex) 
            {
                return false;          
            }        

            if(key != toVertex) 
            {
                vector<edaDAGEdge *> edgeList = vertex->getOutEdges();
                for(unsigned int i = 0; i < edgeList.size(); i++)     
                {  
                    edaDAGVertex* nextVertex = edgeList[i]->getDestVertex();
                    stack.push_back(nextVertex);
                }      
            }
        } while (!stack.empty());

        //Checking right to left
        stack.push_back(vertices[toVertex]);
        do 
        { 
            edaDAGVertex *vertex = stack.back();
            stack.pop_back();
            unsigned int key = vertex->getKey(); 
            if(vertex->getInDegree() == 0 && key != fromVertex) 
            {
                return false;
            }        
            if(key != fromVertex) 
            {
                vector<edaDAGEdge *> edgeList = vertex->getInEdges();
                for(unsigned int i = 0; i < edgeList.size(); i++)     
                {  
                    edaDAGVertex* nextVertex = edgeList[i]->getSourceVertex();
                    stack.push_back(nextVertex);
                }      
            }
        } while (!stack.empty());
        stack.clear();
        return true;
    }    
    
   
        
private:
    map<unsigned int, edaDAGVertex *> vertices;
    map<unsigned int, edaDAGEdge *> edges;
    map<unsigned int, edaDAGEdge *> loops;
    map<unsigned int, DataType> data;
};

#endif
