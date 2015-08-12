/* 
 * File:   edaSerialize.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 7:10 PM
 */

#ifndef EDASERIALIZE_H
#define	EDASERIALIZE_H


#include "edaDefine.h"
#include "edaBuffer.h"


class edaSerialize : public edaObject {
public:
   
    edaSerialize() {}
    
    virtual ~edaSerialize() {}
    
    virtual void serialize(edaBuffer &buf, bool pack) = 0;
    
    void pack(edaBuffer &buf) 
    {
        unsigned int clsid = getClassID();
        buf.pack( &clsid );
        serialize(buf, true);
    }    
    
    virtual void easer() {}
    
    virtual void init() {}
    
    virtual edaSerialize* clone() const = 0;
    
    setClassID( _SYSCLASSID_ + _CLSID_INVALID_ );
    
    virtual const char* className() const = 0;
};
edaSerialize *classGenerate(unsigned int clsid);
edaSerialize *userClassGenerate(unsigned int clsid);

inline edaSerialize *unpack( edaBuffer &buf )
{
    unsigned int clsid;
    edaSerialize *obj;

    // unpack the ID
    buf.unpack( &clsid );

    // generate the object
    obj = classGenerate( clsid );
    obj->serialize( buf, false );

    // return
    return obj;
}



#endif	/* EDASERIALIZE_H */

