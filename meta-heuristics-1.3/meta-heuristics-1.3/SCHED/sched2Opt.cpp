/* 
 * File:   sched2Opt.cpp
 * Author: Trung Nghia
 * 
 * Created on March 19, 2014, 10:10 PM
 */

#include "sched2Opt.h"
#include "schedFitness.h"
sched2Opt::sched2Opt() : schedNeighbour()
{}

sched2Opt::sched2Opt(const schedSolution& route)
{
    schedNeighbour::set(route);
}

sched2Opt::~sched2Opt() 
{
    easer();
}

void sched2Opt::init() 
{
    checkError();
    _first = 0;
    _second = _first + 2;
    _gain = NAN;
}

void sched2Opt::print(ostream& os) const 
{
    checkError();
    cout << "2-opt [" << _first << "; " << _second << "]";  
}

void sched2Opt::update(edaSolution& sol) 
{
    checkError();  
    
    schedSolution& route = (schedSolution&) sol;    
    route.checkError();
    
    double fitness = route.evaluate();
    
    if(route == *_route)
    {        
        for(unsigned int i = 1; i <= (_second + 1 - _first)/2; i++)
        {           
            eda::swap( route[_first + i], route[_second - i + 1] );
        }
        route.set(route.evaluate());
    }
    else
    {
        throw edaException(this, "The input solution is not same the solution in the neighbor !");
    }
}

sched2Opt* sched2Opt::clone() const 
{
    sched2Opt* move = new sched2Opt();
    move->_first = _first;
    move->_second = _second;
    move->_gain = _gain;
    move->_route = _route;
    return move;
}

void sched2Opt::serialize(edaBuffer& buf, bool pack) {
    schedNeighbour::serialize(buf, pack);
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

double sched2Opt::evaluate() 
{    
    checkError();
//    if( eda::isNAN(_gain) )
//    {
//
//        unsigned  pro.distance( _route->at(_first), _route->at(_first + 1) );
//        _gain += pro.distance( _route->at(_second), _route->at( (_second + 1) % size ));
//        _gain -= pro.distance( _route->at(_first), _route->at(_second) );
//        _gain -= pro.distance( _route->at(_first + 1), _route->at( (_second + 1) % size ));
//    }
//    PRINT_DEBUG
    return _route->getFitness();

}


void sched2Opt::next() 
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

void sched2Opt::rand() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _first = eda::rnd.random(0, size - 4);
    _second = eda::rnd.random(_first + 2, size - 1); 
}

sched2Opt& sched2Opt::operator =(const edaNeighbour& neighb) 
{
    const sched2Opt& opt = (sched2Opt&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _first = opt._first;
    _second = opt._second;
    return *this;
}

const char* sched2Opt::className() const 
{
    return "sched2Opt";
}

bool sched2Opt::operator ==(const edaNeighbour& neighb) const 
{
    const sched2Opt& opt = (sched2Opt&) neighb;
    return (_first == opt._first && _second == opt._second);
}
