/* 
 * File:   schedInsertion.cpp
 * Author: Trung Nghia
 * 
 * Created on April 15, 2014, 10:05 AM
 */

#include "schedInsertion.h"
#include "schedFitness.h"

schedInsertion::schedInsertion(unsigned int lambda) : schedNeighbour()
{
    schedInsertion::set(lambda);
}

const char* schedInsertion::className() const 
{
    return "schedInsertion";
}



double schedInsertion::evaluate()
{
   checkError();
	 return _route->getFitness();

}
schedInsertion::schedInsertion(const schedSolution& route, unsigned int lambda)
{
    schedNeighbour::set(route);
    schedInsertion::set(lambda);
}

schedInsertion::~schedInsertion() 
{
    schedNeighbour::easer();
}

void schedInsertion::set(unsigned int lambda) 
{
    if(lambda == 0)
    {
        throw edaException(this, "The number city insert must be large than 0 !");
    }
    _lambda = lambda;
}

void schedInsertion::checkError() const
{
    schedNeighbour::checkError();
    if(_lambda == 0)
    {
        throw edaException(this, "The number city insert must be large than 0 !");
    }
    if(_route != NULL && _lambda > _route->size() - 2)
    {
        throw edaException(this, "The lambda operator must be less than the last index of cities !");
    }
}

void schedInsertion::update(edaSolution& sol) 
{
    checkError();    
    schedSolution& route = (schedSolution&) sol;
    route.checkError();
    double fitness = route.evaluate();
    if(route == *_route)        
    {
        unsigned int city = eda::FLAG;
        unsigned int size =  route.size();
        
        if(_from < _to)
        {
            for(unsigned int i = 0; i < _lambda; i++ )
            {
                city = route[_from];
                route.erase(route.begin() + _from);
                route.insert(route.begin() + _to - 1, city);
            }
        }
        else 
        {
            for(unsigned int i = 0; i < _lambda; i++ )
            {
                if(_to + i < size - 1 && _from + i < size)
                {   
                    city = route[_from + i];
                    route.erase(route.begin() + _from + i);
                    route.insert(route.begin() + _to + i, city);    
                }
                else
                {   
                    city = route[0];
                    route.erase( route.begin() );
                    route.insert(route.begin() + _to - _from  + size - 1, city);
                }
            }
        }  
        route.set(fitness + _gain);
    }
    else
    {
        throw edaException(this, "The input solution is not same the solution in the neighbor !");
    }
}

void schedInsertion::next() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    unsigned int from = _from == 0 ? size : _from; 
    if ( (_from == size - 1) && (_to == from - 1) )
    {
        _from = 0;
        _to = _from + _lambda + 1;
    }
    else 
    {
        if ( _to == from - 1)
        {
            _from++;
            _to = (_from + _lambda + 1) % size  ;
        }   
        else
        {
            _to = (_to + 1) % size;   
        }
    }  
}

void schedInsertion::rand() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _from = eda::rnd.random(0, size - 1);
    _to = ( _from + _lambda + 1 + eda::rnd.random(0, size - _lambda - 2) )% size; 
}

void schedInsertion::init()
{
    checkError();
    _from = 0;
    _to = _from + _lambda + 1;
    _gain = NAN;
}

void schedInsertion::print(ostream& os) const 
{
    checkError();
    cout << _lambda << "-insert [" << _from << "; " << _to << "]";  
}

void schedInsertion::serialize(edaBuffer& buf, bool pack) 
{
    schedNeighbour::serialize(buf, pack);
    if(pack)
    {
        buf.pack( &_from );
        buf.pack( &_to);
        buf.pack( &_lambda);
    }
    else
    {
        buf.unpack( &_from );
        buf.unpack( &_to);
        buf.unpack( &_lambda);
    }
}

schedInsertion& schedInsertion::operator =(const edaNeighbour& neighb) 
{
    const schedInsertion& opt = (schedInsertion&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _from = opt._from;
    _to = opt._to;
    _lambda = opt._lambda;
    return *this;
}

bool schedInsertion::operator ==(const edaNeighbour& neighb) const 
{
    const schedInsertion& opt = (schedInsertion&) neighb;
    return (_from == opt._from && _to == opt._to);
}

schedInsertion* schedInsertion::clone() const 
{
    schedInsertion* move = new schedInsertion();
    move->_from = _from;
    move->_to = _to;
    move->_gain = _gain;
    move->_route = _route;
    move->_lambda = _lambda;
    return move;
}

