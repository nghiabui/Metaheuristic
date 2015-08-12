/* 
 * File:   tspProblem.cpp
 * Author: Tieu Minh
 * 
 * Created on March 11, 2014, 8:31 PM
 */

#include "tspProblem.h"
#include <fstream>

tspProblem::tspProblem() 
{
    _numVert = 0;
    _vectCoord = NULL;
    strcpy(_filename, "");
}   

tspProblem::tspProblem(const char* filename)   
{
    _vectCoord = NULL;
    load(filename);
}

tspProblem::~tspProblem() 
{
    easer();
}

unsigned int tspProblem::size() const 
{ 
    return _numVert;
}

double tspProblem::distance(unsigned int from, unsigned int to) const 
{    
    double distX = (double)(_vectCoord[from * 2] - _vectCoord[to * 2]);
    double distY = (double)(_vectCoord[from * 2 + 1] - _vectCoord[to * 2 + 1]);
    return sqrt(distX * distX + distY * distY) ;
}

void tspProblem::easer() 
{
    _numVert = 0;
    
    strcpy(_filename, "");
    
    if(_vectCoord != NULL)
        delete[] _vectCoord;
        
    _vectCoord = NULL;
}


void tspProblem::load(const char* filename) 
{  
    // Clean old data
    easer();
    
    // Get file name
    strcpy(_filename, filename);
    
    // Read file
    ifstream file(_filename);
    if (file) 
    { 
        cout << ">> Loading [" << _filename << "]" << endl;
        file >> _numVert;
        _vectCoord = new double[_numVert * 2];
        for (unsigned int i = 0; i < _numVert; i++) 
            file >> _vectCoord[i * 2] >> _vectCoord[i *  2 + 1]; 
        file.close();
    }
    else 
    {
        string error = "The file name '";
        error += filename;
        error += "' is not existing !";
        throw edaException( this , error );
    }
}

double tspProblem::x(unsigned int id) const 
{   
    return _vectCoord[id * 2];
}

double tspProblem::y(unsigned int id) const 
{   
    return _vectCoord[id * 2 + 1];
}

tspProblem* tspProblem::clone() const 
{
    tspProblem* result = new tspProblem();
    
    strcpy( result->_filename, _filename);
    
    result->_numVert = _numVert;    
    result->_vectCoord = new double[this->_numVert * 2];
    
    memcpy(result->_vectCoord, _vectCoord, sizeof(double) * 2 * _numVert);
    
    return result;
}

void tspProblem::print(ostream& os) const
{
    checkError();
    os  << ">> File [" << _filename << "]" << endl;    
    os  << "ID " << "\t" << "X " 
        << "\t"  << "Y " << endl;    
    
    for (unsigned int i = 0; i < _numVert; i++)
    {
      os << i << "\t" << _vectCoord[i * 2] 
              << "\t" << _vectCoord[i *  2 + 1] 
              << endl;
    }
}

void tspProblem::serialize(edaBuffer& buf, bool pack) 
{
    if(pack) 
    {
        buf.pack( &_numVert );
        buf.pack( _vectCoord, 2 * _numVert );
        buf.pack( _filename, 256);
    }    
    else 
    {
        easer();        
        buf.unpack( &_numVert );        
        _vectCoord = new double[ 2 * _numVert ];
        buf.unpack( _vectCoord, 2 * _numVert );        
        buf.unpack( _filename, 256);
    }  
}

const char* tspProblem::className() const {
    return "tspProblem";
}

void tspProblem::checkError() const {
    if( strcmp(_filename,"") == 0 )
        throw edaException(this, "Not load input problem file !");
    if( _numVert == 0 )
        throw edaException(this, "The input problem doesn't have the stops !");
}
