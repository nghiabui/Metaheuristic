/* 
 * File:   eddaMath.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 11:35 PM
 */

#ifndef EDAMATH_H
#define	EDAMATH_H


#include <cmath>

#include "edaObject.h"


class edaMath : public edaObject {
public:
    edaMath() {
    }
    
    ~edaMath() {
    }
    
    double round(double value, unsigned int digit) {  
        unsigned long pow10 = 1;
        for( unsigned int i = 0; i < digit; i++ ) pow10 *= 10; 
        double result = ::round ( value * pow10  ) / pow10 ; 
        return result == -0 ? 0 : result;
    }
    
    double* linspace(double a, double b, unsigned int n) {
        double *result = new double[n];
        for(unsigned int i = 0; i < n; i++) {
            result[i] = i * (b - a)/(n - 1);
        }    
        return result;
    }
    
    unsigned int sum(vector<unsigned int> v) {
        unsigned int result = 0;
        for(unsigned int i = 0; i < v.size(); i++) {
            result += v[i];
        }    
        return result;      
    }
    
    double sum(vector<double> v) {
        double result = 0.0;
        for(unsigned int i = 0; i < v.size(); i++) {
            result += v[i];
        }    
        return result;      
    }
    
    const char* className() const {
        return "edaMath";
    }
};

#endif	/* EDAMATH_H */

