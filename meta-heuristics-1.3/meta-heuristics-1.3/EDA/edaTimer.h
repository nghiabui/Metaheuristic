/* 
 * File:   edaTimer.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 10:10 PM
 */

#ifndef EDATIMER_H
#define EDATIMER_H

#include <sys/time.h>

#include "edaObject.h"


class edaTimer : public edaObject {
public:
    edaTimer() {
        d_acc = 0;
    }
    
    ~edaTimer() {}
    
    void start( bool reset = false ) {
        if ( reset )
            d_acc = 0.0;
        gettimeofday( &d_from, 0 ); 
    }
    double stop() {
        gettimeofday( &d_to, 0 );
        double d = duration();
        d_acc += d;
        return d;
    }
    double restart() {
        double d = stop();
        start(true);
        return d;
    }
    double elapse() const {
        return d_acc;     
    }
    double duration() const {
        double _from = (double)d_from.tv_usec * 1e-6 + (double)d_from.tv_sec;
        double _to = (double)d_to.tv_usec * 1e-6 + (double)d_to.tv_sec;
        return _to - _from; 
    }
    
    const char* className() const {
        return "edaTimer";
    }
    
private:
    struct timeval d_from, d_to;
    double d_acc;
};
inline std::ostream & operator << ( std::ostream &os, const edaTimer &t )
{
  os << t.elapse() << " sec(s)" << std::flush;
  return os;
}
#endif /* EDATIMER_H */
