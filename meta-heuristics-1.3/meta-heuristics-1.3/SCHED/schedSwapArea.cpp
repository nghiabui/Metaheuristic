/* 
 * File:   schedSwapArea.cpp
 * Author: Trung Nghia
 * 
 * Created on March 19, 2014, 10:10 PM
 */

#include "schedSwapArea.h"
#include "schedFitness.h"
schedSwapArea::schedSwapArea() : schedNeighbour()
{}

schedSwapArea::schedSwapArea(const schedSolution& route)
{
}

schedSwapArea::~schedSwapArea()
{
    easer();
}

void schedSwapArea::init()
{
    checkError();
    _first = 0;
    _second = _first + 2;
    _gain = NAN;
}

void schedSwapArea::print(ostream& os) const
{
    checkError();
    cout << "swap [" << _first << "; " << _second << "]";
}

void schedSwapArea::update(edaSolution& sol)
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

schedSwapArea* schedSwapArea::clone() const
{
    schedSwapArea* move = new schedSwapArea();
    move->_first = _first;
    move->_second = _second;
    move->_gain = _gain;
    move->_route = _route;
    return move;
}

void schedSwapArea::serialize(edaBuffer& buf, bool pack) {
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

double schedSwapArea::evaluate()
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


void schedSwapArea::next()
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

void schedSwapArea::rand()
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _first = eda::rnd.random(0, size - 4);
    _second = eda::rnd.random(_first + 2, size - 1); 
}

schedSwapArea& schedSwapArea::operator =(const edaNeighbour& neighb)
{
    const schedSwapArea& opt = (schedSwapArea&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _first = opt._first;
    _second = opt._second;
    return *this;
}

const char* schedSwapArea::className() const
{
    return "schedSwapArea";
}

bool schedSwapArea::operator ==(const edaNeighbour& neighb) const
{
    const schedSwapArea& opt = (schedSwapArea&) neighb;
    return (_first == opt._first && _second == opt._second);
}
