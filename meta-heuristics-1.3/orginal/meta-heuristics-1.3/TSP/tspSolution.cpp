/* 
 * File:   tspSolution.cpp
 * Author: Tieu Minh
 * 
 * Created on March 11, 2014, 10:10 PM
 */


#include "tspSolution.h"
#include "tspGenne.h"
#include <iterator>
#include <algorithm>


tspSolution::tspSolution() 
{
    _fitness = NAN;
    _graph = NULL;
}

tspSolution::~tspSolution() 
{    
    easer();
}

void tspSolution::set(const edaProblem& pro) 
{
    tspProblem& graph = (tspProblem&) pro; 
    _graph = &graph; 
}

double tspSolution::evaluate() 
{
    checkError();
    if (eda::isNAN(_fitness)) 
    {
        _fitness = 0;
        unsigned int size = _graph->size();
        for (unsigned int i = 0; i < size; i++) 
          _fitness -= _graph->distance( (*this)[i], (*this)[(i + 1) % size]);
    }        
    return _fitness;
}

void tspSolution::init() 
{
    graphError();
    
    clear();
    unsigned int size = _graph->size();
    for (unsigned int i = 0; i < size; i++) 
    {
        push_back(i);
    }
    // Swap cities
    for (unsigned int i = 0; i < size; i++)
    {
        unsigned int j = eda::rnd.random(size);
        eda::swap( at(i), at(j) );
    }
    // Set NAN vale for route length
    set(NAN);
}

tspSolution* tspSolution::clone() const 
{
    tspSolution* route = new tspSolution();
    route->vector<unsigned int>::operator =(*this);
    route->_graph = _graph;
    route->_fitness = _fitness;    
    return route;
}

void tspSolution::print(ostream& os) const 
{
    checkError();    
    copy( begin(), end() , ostream_iterator<int>( os, " " ) );
}

void tspSolution::set(const double fitness)
{
    _fitness = fitness;
}

void tspSolution::easer()
{
    _fitness = NAN;
    _graph = NULL;
    clear();
}

const tspProblem& tspSolution::pro() const 
{
    graphError();
    
    return *_graph;
}

tspSolution& tspSolution::operator =(const edaSolution& sol) 
{
    const tspSolution& route = (tspSolution&) sol;
    vector<unsigned int>::operator =(route);
    _fitness = route._fitness;  
    _graph = route._graph; 
    return *this;
}

tspSolution::tspSolution(const edaProblem& pro) 
{
    _fitness = NAN;    
    set(pro);
}

void tspSolution::save(const char* filename) 
{
    checkError();
    
    ofstream file;
    file.open(filename);
    
    unsigned int size = this->size();
    for(unsigned int i = 0; i < size; i++)
        file << _graph->x(at(i)) << " ";
    file << endl;
    
    for(unsigned int i = 0; i < size; i++)
        file << _graph->y(at(i)) << " ";
    file.close();
}

void tspSolution::serialize(edaBuffer& buf, bool pack) 
{
    unsigned int n = size();
    unsigned int value;
    if(pack) 
    {
        buf.pack( &n );
        for(unsigned int i = 0; i < n; i++) 
        {
            value = at(i);
            buf.pack( &value );
        }
        buf.pack( &_fitness );

    }    
    else 
    {
        easer();        
        buf.unpack( &n );
        for(unsigned int i = 0; i < n; i++) 
        {
            buf.unpack( &value );
            push_back( value );
        }
        buf.unpack( &_fitness );
    }  
}

bool tspSolution::load(const char* filename) 
{
    graphError();
    
    ifstream file(filename);
    if( file ) 
    {
        clear();    
        unsigned int size = _graph->size();
        resize(size);
        for (unsigned int i = 0; i < size; i++) 
        {
            file >> at(i);
        }
        file.close();
        set(NAN);
    }
    else
    {
        string error = "The file name '";
        error += filename;
        error += "' is not existing !";
        throw edaException( this , error );
    }
    return false;
}

bool tspSolution::operator == (const edaSolution& sol) const 
{
    const tspSolution& route = (tspSolution&) sol;
    return equal( begin(), end(), route.begin() );
}

const char* tspSolution::className() const {
    return "tspSolution";
}

void tspSolution::checkError() const 
{
    graphError();
    
    initError();    
}

void tspSolution::graphError() const 
{
    if(_graph == NULL)
        throw edaException(this, "Not set graph !");
    _graph->checkError();
}

void tspSolution::initError() const
{
    if(size() == 0)
        throw edaException(this, "Not initial the route !");
    if(size() != _graph->size())
        throw edaException(this, "The route not satisfy the graph !");
}

void tspSolution::decode(const edaChromosome& chro)
{
    unsigned int size = chro.length();
    for(unsigned int i = 0; i < size; i++ )
    {
        tspGenne* genne = (tspGenne*)chro.at(i);
        at(i) = *genne;
    }
    _fitness = NAN;
}

edaChromosome* tspSolution::encode() const
{
    edaChromosome* chro = new edaChromosome();
    unsigned int size = this->size();
    for(unsigned int i = 0; i < size; i++ )
    {
        tspGenne genne = at(i);
        chro->add(genne);
    }
    return chro;
}

