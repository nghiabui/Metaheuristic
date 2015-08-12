/* 
 * File:   tspGenne.cpp
 * Author: Tieu Minh
 * 
 * Created on April 23, 2014, 9:37 AM
 */

#include "tspGenne.h"


tspGenne::tspGenne(unsigned int value)
{
    _value = value;
}

tspGenne::~tspGenne() 
{
}

const char* tspGenne::className() const
{
    return "tspGenne";
}

void tspGenne::print(ostream &os) const 
{
    os << _value;
}

tspGenne& tspGenne::operator = (unsigned int value)
{
    _value = value;
    return *this;
}

tspGenne& tspGenne::operator = (const edaGenne &genne) 
{
     tspGenne& city = (tspGenne&) genne;
     _value = city._value;
     return *this;
}

bool tspGenne::operator == (const edaGenne &genne) const
{
    const tspGenne& city = (tspGenne&) genne; 
    return _value == city._value;
}

tspGenne* tspGenne::clone() const
{
    tspGenne* genne = new tspGenne();
    genne->_value = _value;
    return genne; 
}

void tspGenne::checkError() const 
{
    if(_value == eda::FLAG)
        throw edaException(this, "The value of genne is not set !");
}

tspGenne::operator unsigned int()
{
    return _value;
}
