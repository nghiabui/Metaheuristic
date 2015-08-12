/* 
 * File:   tsp2Opt.cpp
 * Author: Tieu Minh
 * 
 * Created on March 19, 2014, 10:10 PM
 */

#include "tsp2Opt.h"

tsp2Opt::tsp2Opt() : tspNeighbour() 
{}

tsp2Opt::tsp2Opt(const tspSolution& route)
{
    tspNeighbour::set(route);
}

tsp2Opt::~tsp2Opt() 
{
    easer();
}

void tsp2Opt::init() 
{
    checkError();
    _first = 0;
    _second = _first + 2;
    _gain = NAN;
}

void tsp2Opt::print(ostream& os) const 
{
    checkError();
    cout << "2-opt [" << _first << "; " << _second << "]";  
}

void tsp2Opt::update(edaSolution& sol) 
{
    checkError();  
    
    tspSolution& route = (tspSolution&) sol;    
    route.checkError();
    
    double fitness = route.evaluate();
    
    if(route == *_route)
    {        
        for(unsigned int i = 1; i <= (_second + 1 - _first)/2; i++)
        {           
            eda::swap( route[_first + i], route[_second - i + 1] );
        }
        route.set(fitness + _gain);
    }
    else
    {
        throw edaException(this, "The input solution is not same the solution in the neighbor !");
    }
}

tsp2Opt* tsp2Opt::clone() const 
{
    tsp2Opt* move = new tsp2Opt();
    move->_first = _first;
    move->_second = _second;
    move->_gain = _gain;
    move->_route = _route;
    return move;
}

void tsp2Opt::serialize(edaBuffer& buf, bool pack) {
    tspNeighbour::serialize(buf, pack);
    if(pack)
    {
        buf.pack( &_first );
        buf.pack( &_second );
    }
    else
    {
        buf.unpack( &_first );
        buf.unpack( &_second );
    }
}

double tsp2Opt::evaluate() 
{    
    checkError();
    if( eda::isNAN(_gain) ) 
    {
        _gain = 0;
        unsigned int size = _route->size();
        const tspProblem& pro = _route->pro();  
        _gain += pro.distance( _route->at(_first), _route->at(_first + 1) );
        _gain += pro.distance( _route->at(_second), _route->at( (_second + 1) % size ));            
        _gain -= pro.distance( _route->at(_first), _route->at(_second) );
        _gain -= pro.distance( _route->at(_first + 1), _route->at( (_second + 1) % size ));    
    }
    return _gain;
}

void tsp2Opt::next() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    if ( (_first == size - 4) && (_second == _first + 3) )
    {
        _first = 0;
        _second = _first + 2;
    }
    else 
    {
        _second++;
        if (_second == size)
        {
          _first++;
          _second = _first + 2;
        }
    }  
}

void tsp2Opt::rand() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _first = eda::rnd.random(0, size - 4);
    _second = eda::rnd.random(_first + 2, size - 1); 
}

tsp2Opt& tsp2Opt::operator =(const edaNeighbour& neighb) 
{
    const tsp2Opt& opt = (tsp2Opt&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _first = opt._first;
    _second = opt._second;
    return *this;
}

const char* tsp2Opt::className() const 
{
    return "tsp2Opt";
}

bool tsp2Opt::operator ==(const edaNeighbour& neighb) const 
{
    const tsp2Opt& opt = (tsp2Opt&) neighb;
    return (_first == opt._first && _second == opt._second);
}
