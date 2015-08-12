/* 
 * File:   edaNaturalSelection.h
 * Author: Tieu Minh
 *
 * Created on April 22, 2014, 3:40 PM
 */

#ifndef EDANATURALSELECTION_H
#define	EDANATURALSELECTION_H

#include "edaOperator.h"


class edaNaturalSelection : public edaOperator 
{
public:
    
    virtual bool update(edaPopulation& pop) = 0;

    virtual void serialize(edaBuffer& buf, bool pack) = 0;    

    virtual edaNaturalSelection* clone() const = 0;    

    virtual void checkError() const = 0;
    
    virtual const char* className() const = 0;


};

#endif	/* EDANATURALSELECTION_H */

