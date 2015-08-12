/* 
 * File:   schedGenne.cpp
 * Author: Trung Nghia
 * 
 * Created on April 23, 2014, 9:37 AM
 */

#include "schedGenne.h"


schedGenne::schedGenne(unsigned int value)
{
    _value = value;
}

schedGenne::~schedGenne() 
{
}

const char* schedGenne::className() const
{
    return "schedGenne";
}

void schedGenne::print(ostream &os) const 
{
    os << _value;
}

schedGenne& schedGenne::operator = (unsigned int value)
{
    _value = value;
    return *this;
}

schedGenne& schedGenne::operator = (const edaGenne &genne) 
{
     schedGenne& city = (schedGenne&) genne;
     _value = city._value;
     return *this;
}

bool schedGenne::operator == (const edaGenne &genne) const
{
    const schedGenne& city = (schedGenne&) genne; 
    return _value == city._value;
}

schedGenne* schedGenne::clone() const
{
    schedGenne* genne = new schedGenne();
    genne->_value = _value;
    return genne; 
}

void schedGenne::checkError() const 
{
    if(_value == eda::FLAG)
        throw edaException(this, "The value of genne is not set !");
}

schedGenne::operator unsigned int()
{
    return _value;
}
