/* 
 * File:   edaSerialize.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 7:10 PM
 */

#ifndef EDABUFFER_H
#define EDABUFFER_H


#include <memory.h>
#include <assert.h>

#include "edaObject.h"
#include "edaDefine.h"

class edaBuffer : public edaObject
{
public:
    edaBuffer() : bufferData(0) {
        reset();
    }
    
    ~edaBuffer() {
        easerArray(bufferData);
    }
    
    void reset() {
        unpackPos = 0;
        easerArray(bufferData);
        bufferData = 0;
        _size = 0;
        _actualSize = 0;
    }
  
    // char
    void pack(const char *data, int count = 1) {
        assert( count > 0 );
        // increase the buffer if needed
        if ( _actualSize + count > _size ){
            // calculate the new size
            int newsize = 1024 * ((_actualSize + count) / 1024 + 1 );
            // allocate memory
            char *newBufferData = new char[newsize];
            // copy old data (if not NULL) to new memory
            if ( bufferData )
              memcpy( newBufferData, bufferData, _actualSize );
            // delete the old memory (if not NULL)
            if ( bufferData )
              delete [] bufferData;
            // set data members to new values
            _size = newsize;
            bufferData = newBufferData;
        }
        // copy new data to the back of new memory
        memcpy( bufferData + _actualSize, data, count );
        // set data members to new values
        _actualSize += count;
    }
    
    void unpack(char *data, int count = 1) {
        assert( count > 0);  
        memcpy(data, ((char *)bufferData) + unpackPos, count);
        unpackPos += count;
    }

    // unsigned char
    void pack(const unsigned char *data, int count = 1) {
        pack((char *)data, count * sizeof(unsigned char));        
    }
    
    void unpack(unsigned char *data, int count = 1) {
        unpack((char *)data, count * sizeof(unsigned char));        
    }

    // bool
    void pack(const bool *data, int count = 1) {
        pack((char *)data, count * sizeof(bool));
    }
    
    void unpack(bool *data, int count = 1) {
        unpack((char *)data, count * sizeof(bool));
    }

    // int
    void pack(const int *data, int count = 1) {
        pack((char *)data, count * sizeof(int));
    }
    
    void unpack(int *data, int count = 1) {
        unpack((char *)data, count * sizeof(int));
    }

    // unsigned
    void pack(const unsigned *data, int count = 1) {
        pack((char *)data, count * sizeof(unsigned));
    }
    
    void unpack(unsigned *data, int count = 1) {
        unpack((char *)data, count * sizeof(unsigned));
    }

    // long
    void pack(const long *data, int count = 1) {
        pack((char *)data, count * sizeof(long));
    }
    
    void unpack(long *data, int count = 1) {
        unpack((char *)data, count * sizeof(long));
    }

    // unsigned long
    void pack(const unsigned long *data, int count = 1) {
        pack((char *)data, count * sizeof(unsigned long));
    }
    
    void unpack(unsigned long *data, int count = 1) {
        unpack((char *)data, count * sizeof(unsigned long));
    }

    // short
    void pack(const short *data, int count = 1) {
        pack((char *)data, count * sizeof(short));
    }
    
    void unpack(short *data, int count = 1) {
        unpack((char *)data, count * sizeof(short));
    }

    // unsigned short
    void pack(const unsigned short *data, int count = 1) {
        pack((char *)data, count * sizeof(unsigned short));
    }
    
    void unpack(unsigned short *data, int count = 1) {
        unpack((char *)data, count * sizeof(unsigned short));
    }

    // float
    void pack(const float *data, int count = 1) {
        pack((char *)data, count * sizeof(float));
    }
    
    void unpack(float *data, int count = 1) {
        unpack((char *)data, count * sizeof(float));
    }

    // double
    void pack(const double *data, int count = 1) {
        pack((char *)data, count * sizeof(double));
    }
    
    void unpack(double *data, int count = 1) {
        unpack((char *)data, count * sizeof(double));
    }

    char *getBuffer() { return bufferData; }

    int getActualSize() const { return _actualSize; }

    void setBuffer( int as, char *buf ) {
        _size = _actualSize = as;
        easerArray(bufferData);
        bufferData = buf;
    }

    const char* className() const {
        return "edaBuffer";
    }

    
private:
    char *bufferData;
    int _size, _actualSize;
    int unpackPos;
};

#endif /* EDABUFFER_H */

