/* 
 * File:   edaWrapperControl.h
 * Author: Tieu Minh
 *
 * Created on April 10, 2014, 9:06 AM
 */

#ifndef EDAWORKFLOW_H
#define	EDAWORKFLOW_H

#include <map>
#include "edaDAG.h"
#include "edaPopulation.h"
#include "edaSynthCollect.h"
#include "edaStopCriteria.h"

class edaWorkflow : public edaObject
{
public:

    edaWorkflow()
        : problem( NULL ) 
    {           
        vertexNum = 0;
        edgeNum = 0;
        loopNum = 0;
    }

    
    edaWorkflow(const edaProblem& pro)
        : problem( pro.clone() ) 
    {           
        vertexNum = 0;
        edgeNum = 0;
        loopNum = 0;
    }

    void set(const edaProblem& pro) 
    {
        pro.checkError();
        easerObject(problem);
        problem = pro.clone();
    }
    
    virtual ~edaWorkflow() 
    {
        easerObject(problem);
        
        map<unsigned int, edaPopulation *>::iterator popIt;
        map<unsigned int, edaCollection *>::iterator clectIt;
        map<unsigned int, edaStopCriteria *>::iterator stopIt;
  
        for( popIt=taskPop.begin(); popIt!=taskPop.end(); popIt++ )
            easerObject(popIt->second);
        for( clectIt=taskSlect.begin(); clectIt!=taskSlect.end(); clectIt++ )
            easerObject(clectIt->second);
        for( stopIt=loopCond.begin(); stopIt!=loopCond.end(); stopIt++) 
            easerObject(stopIt->second);        
    }

    virtual unsigned int insertVertex(const edaSearch& sa) 
    {
        edaSynthCollect clect; 
        return insertVertex( sa, clect );
    }

    virtual unsigned int insertVertex(const edaSearch& sa, const edaCollection &clect) 
    {
        unsigned int id = vertexNum;     
        sa.checkError();
        edaSearch* search = sa.clone(); 
        search->TaskID = id; 
        taskDAG.insertVertex(id, search);
        taskSlect[id] = clect.clone();
        taskStatus[id] = STATUS_PENDING;  
        vertexNum++;
        return id;
    }

    virtual unsigned int insertEdge(const unsigned int from, const unsigned int to) 
    {
        unsigned int id = edgeNum;        
        taskDAG.insertEdge(id, from, to);
        edgeNum++;
        return id;
    }

    virtual unsigned int insertLoop(const unsigned int from, const unsigned int to, const edaStopCriteria &con) 
    {
        unsigned int id = loopNum;
        taskDAG.insertLoop(id, from, to);        
        loopCond[id] = con.clone();
        loopCond[id]->init();
        loopStatus[id] = STATUS_PENDING;
        loopNum++;      
        return id;
    }

    virtual void search(edaPopulation &pop) = 0;    
    
    virtual const char* className() const 
    {
        return "edaWorkflow";
    }
    
    edaPopulation* resultOfTaskID(unsigned int taskID) 
    {
        edaPopulation* pop = NULL;
        if( taskID > taskPop.size() )
            throw edaException(this, "The taskID is not correct !");
        pop = taskPop[taskID];
        return pop;
    }
    
 
    
protected:

    virtual void checkError() const 
    {
        if (problem ==  NULL)
            throw edaException(this, "Not set the problem for workflow !");
    }
    
    virtual vector<unsigned int> findReadyTask() const 
    {
        vector<unsigned int> taskList;
        vector<unsigned int> readyTasks;
        vector<unsigned int>::iterator intIter;

        taskList = taskDAG.traverse();
        for (intIter = taskList.begin(); intIter != taskList.end(); intIter++)
        {
          map<unsigned int, unsigned int>::const_iterator mci = taskStatus.find(*intIter);
          if (mci->second == STATUS_READY || mci->second == STATUS_LOOP_READY)
          {
            // If it is ready, then add to ready list
            readyTasks.push_back(*intIter);
          }
        }

        return readyTasks;
    }
    
    virtual unsigned int checkTaskStatus( unsigned int taskID) 
    {
        if (taskStatus[taskID] != STATUS_PENDING && taskStatus[taskID]!= STATUS_LOOPING)
        {
            return taskStatus[taskID];
        }

        vector<unsigned int> parentTasks;
        vector<unsigned int>::iterator parentIter;

        parentTasks = taskDAG.getParentNodes( taskID );
        if ( parentTasks.empty() )
        {
            // If node has no parent, then it is ready
            if(taskStatus[taskID] == STATUS_PENDING)
                return STATUS_READY;
            if (taskStatus[taskID] == STATUS_LOOPING) 
                return STATUS_LOOP_READY;
        }
        // If loop mode 
        else if (taskStatus[taskID] == STATUS_LOOPING) 
            return STATUS_LOOP_READY;

        bool allFail = true;

        // Check for its parents
        for (parentIter = parentTasks.begin(); parentIter != parentTasks.end(); parentIter++)
        {
            unsigned int jobStatus = taskStatus[*parentIter];

            if ((jobStatus != STATUS_FINISHED) && (jobStatus != STATUS_ERROR))
            {
                // If node's parent is not finished yet, then it is pending
                return STATUS_PENDING;
            }

            if (jobStatus != STATUS_ERROR)
            {
                allFail = false;
            }
        }

        if ( allFail )
        {
            cout << taskStatus[taskID] << endl;
            // If all its parents is fail, then it is fail too
            return STATUS_ERROR;
        }

        // Otherwise, it is ready to run
        return STATUS_READY;   
    }
    
    virtual unsigned int checkLoopStatus( unsigned int taskID) 
    {
        unsigned int count = 0;   
        for(edaDAGEdge* loop = taskDAG.findLoop(taskID, loopStatus);
            loop != NULL; loop = taskDAG.findLoop(taskID, loopStatus) )
        {
            count ++;

            edaStopCriteria * con = loopCond[loop->getKey()];
            edaPopulation* pop = taskPop[taskID];
            if(!con->check(pop)) 
            {     
                loopStatus[loop->getKey()] = STATUS_FINISHED;     
            }
            else 
            {
                edaCluster* cluster = taskDAG.createCluster(loop);
                for(unsigned int i = 0; i < cluster->size(); i++) 
                {
                    taskStatus[cluster->at(i)->getKey()] = STATUS_PENDING;
                }
                vector<edaDAGEdge*> subLoop = taskDAG.detectSubLoop(*loop);
                for(unsigned int i = 0; i < subLoop.size(); i++) 
                {
                    unsigned int loopID = subLoop[i]->getKey();
                    loopStatus[loopID] = STATUS_PENDING;
                    loopCond[loopID]->init();
                }
                easerObject(cluster);
                edaDAGVertex *toVertex = loop->getDestVertex();
                taskStatus[toVertex->getKey()] = STATUS_LOOPING; 
                return count;
            }
        }
        return count;  
    }
    
    
    virtual edaPopulation* chooseSolution( unsigned int taskID, edaPopulation &pop )  
    {
        vector<unsigned int> parentTasks;
        parentTasks = taskDAG.getParentNodes(taskID);
        edaPopulation *Pop = NULL;   
        if ( taskStatus[taskID] == STATUS_LOOP_READY ) 
        {
            edaDAGEdge* loop = taskDAG.detectLoop(taskID, loopStatus); 
            int loopKey = loop->getSourceVertex()->getKey();
            Pop = taskPop[loopKey]->clone();
            taskSlect[taskID]->select(*Pop);        
        }
        else if (parentTasks.empty())
        {
            // If no parent, use input solution
            Pop = pop.clone();
            taskSlect[taskID]->select(*Pop);
        }
        else
        {
            // Have parents
            Pop = new edaPopulation();
            vector<unsigned int>::iterator parentIter;
            for (parentIter = parentTasks.begin(); parentIter != parentTasks.end(); parentIter++) 
            {
                if (taskPop[*parentIter] != NULL) 
                {
                    edaPopulation *p = taskPop[*parentIter];
                    for(unsigned int i = 0; i < p->size(); i++) 
                    {
                        Pop->add( (p->at(i))->clone() );
                    }
                    taskSlect[taskID]->select(*Pop);
                }
            }
        }
        return Pop;
    }
    
    bool allDone() 
    {
        vector<unsigned int> taskList;
        vector<unsigned int>::iterator intIter;
        taskList = taskDAG.traverse();
        bool allDone;
        bool haveError;

        // Scan until no new error found
        do
        {
            allDone = true;
            haveError = false;

            for (intIter = taskList.begin(); intIter != taskList.end(); intIter++)
            {
                unsigned int taskID = *intIter;
                // Update all nodes status
                unsigned int oldStatus = taskStatus[taskID];
                unsigned int newStatus = checkTaskStatus( taskID );

                if ((newStatus != oldStatus) && (newStatus == STATUS_ERROR))
                {
                    haveError = true;
                }
                taskStatus[taskID] = newStatus;

                if ((taskStatus[taskID] != STATUS_FINISHED)
                    && (taskStatus[taskID] != STATUS_ERROR))
                {
                    allDone = false;
                }
            }
        } while (haveError && !allDone);


        map<unsigned int, unsigned int>::const_iterator loopIter;
        for (loopIter = loopStatus.begin(); loopIter != loopStatus.end(); loopIter++) 
        {
            if(loopIter->second != STATUS_FINISHED) 
            {
                return false;
            }
        }

        return allDone;
    }
    
    unsigned int detectBestResultTaskID() const
    {
        map<unsigned int, edaPopulation *>::const_iterator popIt;
        unsigned int id = eda::FLAG;
        double max = - eda::INF, value;
        for( popIt=taskPop.begin(); popIt!=taskPop.end(); popIt++ ) 
        {
            value = popIt->second->max();
            if(value > max)
            {
                max = value;
                id = popIt->first;
            }
        }
        return id;
    }
    
    map<unsigned int, edaCollection *> taskSlect;    
    map<unsigned int, edaStopCriteria*> loopCond; 
    map<unsigned int, edaPopulation*> taskPop;
    
    map<unsigned int, unsigned int> taskStatus;
    map<unsigned int, unsigned int> loopStatus;
    
    edaDAG<edaSearch*> taskDAG; 
    unsigned int vertexNum;
    unsigned int loopNum;
    unsigned int edgeNum;
    edaProblem* problem;
};

#endif	/* EDAWORKFLOW_H */

