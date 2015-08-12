/* 
 * File:   edaNeighbour.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 7:06 PM
 * Base class for exception
 */

#ifndef EDAEXCEPTION_H
#define EDAEXCEPTION_H

#include "edaObject.h"


#include <string>

class edaException : public edaObject {
public:
 
    edaException(const edaObject* obj, std::string _errorMessage) {
        errorMessage = obj->className();
        errorMessage += ": ";
        errorMessage += _errorMessage;
        errorMessage += "\n";
    }
    
    ~edaException() {}
    
    edaException(const edaObject& obj, std::string _errorMessage) {
        edaException( &obj, _errorMessage );
    }
    
    const std::string &getErrorMessage() {
        return errorMessage;
    }
    
    const char* className() const {
        return "edaException";
    }
	
private:
    std::string errorMessage;
};

#endif /* EDAEXCEPTION */
