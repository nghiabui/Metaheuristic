/* 
 * File:   tspInsertion.cpp
 * Author: Tieu Minh
 * 
 * Created on April 15, 2014, 10:05 AM
 */

#include "tspInsertion.h"


tspInsertion::tspInsertion(unsigned int lambda) : tspNeighbour() 
{
    tspInsertion::set(lambda);
}

const char* tspInsertion::className() const 
{
    return "tspInsertion";
}

double tspInsertion::evaluate() 
{
    checkError();
    if( eda::isNAN(_gain) ) 
    {
        _gain = 0;
        unsigned int size = _route->size();
        const tspProblem& pro = _route->pro();  
        
        unsigned int p1 = _from;
        unsigned int p2 = _from == 0 ? size - 1 : _from - 1;
        unsigned int p3 = _to; 
        unsigned int p4 = _to == 0 ? size - 1 : _to - 1;
        unsigned int p5 = (_from + _lambda - 1) % size;
        unsigned int p6 = (_from + _lambda) % size;
        
        _gain += pro.distance( _route->at(p1), _route->at(p2) );
        _gain += pro.distance( _route->at(p3), _route->at(p4) ); 
        _gain += pro.distance( _route->at(p5), _route->at(p6) ); 
 
        _gain -= pro.distance( _route->at(p4), _route->at(p1) );
        _gain -= pro.distance( _route->at(p5), _route->at(p3) );  
        _gain -= pro.distance( _route->at(p2), _route->at(p6) ); 

    }
    return _gain;
}

tspInsertion::tspInsertion(const tspSolution& route, unsigned int lambda) 
{
    tspNeighbour::set(route);
    tspInsertion::set(lambda);
}

tspInsertion::~tspInsertion() 
{
    tspNeighbour::easer();
}

void tspInsertion::set(unsigned int lambda) 
{
    if(lambda == 0)
    {
        throw edaException(this, "The number city insert must be large than 0 !");
    }
    _lambda = lambda;
}

void tspInsertion::checkError() const
{
    tspNeighbour::checkError();
    if(_lambda == 0)
    {
        throw edaException(this, "The number city insert must be large than 0 !");
    }
    if(_route != NULL && _lambda > _route->size() - 2)
    {
        throw edaException(this, "The lambda operator must be less than the last index of cities !");
    }
}

void tspInsertion::update(edaSolution& sol) 
{
    checkError();    
    tspSolution& route = (tspSolution&) sol;
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

void tspInsertion::next() 
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

void tspInsertion::rand() 
{
    checkError();
    _gain = NAN;
    unsigned int size = _route->size();
    _from = eda::rnd.random(0, size - 1);
    _to = ( _from + _lambda + 1 + eda::rnd.random(0, size - _lambda - 2) )% size; 
}

void tspInsertion::init()
{
    checkError();
    _from = 0;
    _to = _from + _lambda + 1;
    _gain = NAN;
}

void tspInsertion::print(ostream& os) const 
{
    checkError();
    cout << _lambda << "-insert [" << _from << "; " << _to << "]";  
}

void tspInsertion::serialize(edaBuffer& buf, bool pack) 
{
    tspNeighbour::serialize(buf, pack);
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

tspInsertion& tspInsertion::operator =(const edaNeighbour& neighb) 
{
    const tspInsertion& opt = (tspInsertion&) neighb;
    _route = opt._route;
    _gain = opt._gain;
    _from = opt._from;
    _to = opt._to;
    _lambda = opt._lambda;
    return *this;
}

bool tspInsertion::operator ==(const edaNeighbour& neighb) const 
{
    const tspInsertion& opt = (tspInsertion&) neighb;
    return (_from == opt._from && _to == opt._to);
}

tspInsertion* tspInsertion::clone() const 
{
    tspInsertion* move = new tspInsertion();
    move->_from = _from;
    move->_to = _to;
    move->_gain = _gain;
    move->_route = _route;
    move->_lambda = _lambda;
    return move;
}

