/* 
 * File:   schedSwap.cpp
 * Author: Trung Nghia
 * 
 * Created on March 19, 2014, 10:10 PM
 */

#include "schedSwap.h"
#include "schedFitness.h"
schedSwap::schedSwap() : schedNeighbour()
{}

schedSwap::schedSwap(const schedSolution& route)
{
}

schedSwap::~schedSwap()
{
    easer();
}

void schedSwap::init()
{
    checkError();
    _first = 0;
    _second = _first + 2;
    _gain = NAN;
}

void schedSwap::print(ostream& os) const
{
    checkError();
    cout << "swap [" << _first << "; " << _second << "]";
}

void schedSwap::update(edaSolution& sol)
{
    checkError();  
    
    schedSolution& route = (schedSolution&) sol;    
    route.checkError();
    
    double fitness = route.evaluate();
    
    if(route == *_route)
    {        

            eda::swap( route[_first ], route[_second]  );

        route.set(route.evaluate());
    }
    else
    {
        throw edaException(this, "The input solution is not same the solution in the neighbor !");
    }
}

schedSwap* schedSwap::clone() const
{
    schedSwap* move = new schedSwap();
    move->_first = _first;
    move->_second = _second;
    move->_gain = _gain;
    move->_route = _route;
    return move;
}

void schedSwap::serialize(edaBuffer& buf, bool pack) {
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

double schedSwap::evaluate()
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


void schedSwap::next()
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

void schedSwap::rand()
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _first = eda::rnd.random(0, size - 4);
    _second = eda::rnd.random(_first + 2, size - 1); 
}

schedSwap& schedSwap::operator =(const edaNeighbour& neighb)
{
    const schedSwap& opt = (schedSwap&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _first = opt._first;
    _second = opt._second;
    return *this;
}

const char* schedSwap::className() const
{
    return "schedSwap";
}

bool schedSwap::operator ==(const edaNeighbour& neighb) const
{
    const schedSwap& opt = (schedSwap&) neighb;
    return (_first == opt._first && _second == opt._second);
}
