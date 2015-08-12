/* 
 * File:   eddaMath.h
 * Author: Tieu Minh
 *
 * Created on March 11, 2014, 11:35 PM
 * 
 * The method for generating gamma variables appears in "A Simple Method
 * for Generating Gamma Variables" by Marsaglia and Tsang, ACM
 * Transactions on Mathematical Software, Vol. 26, No 3, Sep 2000, pages
 * 363-372.
 */

#ifndef EDARNG_H
#define EDARNG_H

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <climits>
#include <vector>

#include "edaObject.h"

using std::vector;

static const double PI   =    3.1415926535897932;
static const double AD_l =    0.6931471805599453;
static const double AD_a =    5.7133631526454228;
static const double AD_b =    3.4142135623730950;
static const double AD_c =   -1.6734053240284925;
static const double AD_p =    0.9802581434685472;
static const double AD_A =    5.6005707569738080;
static const double AD_B =    3.3468106480569850;
static const double AD_H =    0.0026106723602095;
static const double AD_D =    0.0857864376269050;

typedef signed int sint;
typedef unsigned int uint;
typedef signed long slong;
typedef unsigned long ulong;
#if ULONG_MAX == 4294967295ul
inline ulong ULONG32(slong x) { return (ulong(x)); }
inline ulong ULONG32(ulong x) { return (ulong(x)); }
inline ulong ULONG32(double x) { return (ulong(x)); }
inline slong UL32toSL32(ulong x) { return (slong(x)); }
#else
inline ulong ULONG32(slong x) { return (ulong(x) & 0xfffffffful); }
inline ulong ULONG32(ulong x) { return (x & 0xfffffffful); }
inline ulong ULONG32(double x) { return (ulong(x) & 0xfffffffful); }
inline slong UL32toSL32(ulong x)
    { return (x < 0x80000000ul ? slong(x) : -1 * (0x80000000ul - (x & 0x7ffffffful))); }
#endif

class edaRNG : public edaObject {
private:
    ulong tm; // Used to ensure different RNGs have different seeds.
    ulong z, w, jsr, jcong; // Seeds

    ulong kn[128], ke[256];
    double wn[128], fn[128], we[256],fe[256];    
    
 public:
    edaRNG() { init(); zigset(); }
    edaRNG(ulong x_) :
        z(x_), w(x_), jsr(x_), jcong(x_) { zigset(); }
    edaRNG(ulong z_, ulong w_, ulong jsr_, ulong jcong_ ) :
        z(z_), w(w_), jsr(jsr_), jcong(jcong_) { zigset(); }
    ~edaRNG() { }

#if ULONG_MAX == 4294967295ul
    // 32 bit unsigned longs
    ulong znew() 
        { return (z = 36969 * (z & 0xfffful) + (z >> 16)); }
    ulong wnew() 
        { return (w = 18000 * (w & 0xfffful) + (w >> 16)); }
    ulong MWC()    
        { return ((znew() << 16) + wnew()); }
    ulong SHR3()
        { jsr ^= (jsr << 17); jsr ^= (jsr >> 13); return (jsr ^= (jsr << 5)); }
    ulong CONG() 
        { return (jcong = 69069 * jcong + 1234567); }
    ulong rand_int32()             // [0,2^32-1]
        { return ((MWC() ^ CONG()) + SHR3()); }
    ulong rand_int()                 // [0,2^32-1]
        { return ((MWC() ^ CONG()) + SHR3()); }
#elif ULONG_MAX == 18446744073709551615ul
#ifdef RNG_C
#warning "Compiling RNG class for 64-bit architecture"
#endif
    // 64-bit unsigned longs
    // This is not as elegant and fast as it could be, but it works.
    ulong znew() 
        { return (z = ((36969 * (z & 0xfffful) + (z >> 16)) & 0xfffffffful)); }
    ulong wnew() 
        { return (w = ((18000 * (w & 0xfffful) + (w >> 16)) & 0xfffffffful)); }
    ulong MWC()    
        { return (((znew() << 16) + wnew()) & 0xfffffffful); }
    ulong SHR3()
        { jsr ^= (jsr << 17); jsr ^= (jsr >> 13); jsr ^= (jsr << 5);
            return (jsr &= 0xfffffffful); }
    ulong CONG() 
        { return (jcong = ((69069 * jcong + 1234567) & 0xfffffffful)); }
    ulong rand_int32()                 // [0,2^32-1]
        { return (((MWC() ^ CONG()) + SHR3()) & 0xfffffffful); }
    ulong rand_int()                     // [0,2^64-1]
        { return (rand_int32() | (rand_int32() << 32)); }
#endif
    double RNOR() {
        slong h = UL32toSL32(rand_int32()), i = h & 127;
        return (((ulong)std::abs(h) < kn[i]) ? h * wn[i] : nfix(h, i));
    }
    double REXP() {
        ulong j = rand_int32(), i = j & 255;
        return ((j < ke[i]) ? j * we[i] : efix(j, i));
    }

    double nfix(slong h, ulong i) {
        const double r = 3.442620; 	// The starting of the right tail

        double x, y;
        for(;;) {
            x = h * wn[i];

            // If i == 0, handle the base strip
            if (i == 0) {
                do {
            x = -log(rand_open01()) * 0.2904764;     // .2904764 is 1/r
            y = -log(rand_open01());
                } while (y + y < x * x);
                return ((h > 0) ? r + x : -r - x);
            }

            // If i > 0, handle the wedges of other strips
            if (fn[i] + rand_open01() * (fn[i - 1] - fn[i]) < exp(-.5 * x * x))
                return x;

            // start all over
            h = UL32toSL32(rand_int32());
            i = h & 127;
            if (ULONG32(std::abs(h)) < kn[i])
                return (h * wn[i]);
        }
    }
    double efix(ulong j, ulong i) {
        for (;;) {
            if (i == 0)
                return (7.69711 - log(rand_open01()));

            const double x = j * we[i];
            if (fe[i] + rand_open01() * (fe[i - 1] - fe[i]) < exp(-x)) 
                return x;

            j = rand_int32();
            i = (j & 255);
            if (j < ke[i])
                return (j * we[i]);	
        }
    }
    void zigset() {
        static bool inited = 0;
        if (inited)
            return;
        inited = 1;

        // Set up tables for RNOR
        const double m1 = 2147483648.0; // 2^31
        const double vn = 9.91256303526217e-3;
        double tn = 3.442619855899;
        double q = vn / exp(-.5 * tn * tn);
        kn[0] = ULONG32((tn / q) * m1); kn[1] = 0;
        wn[0] = q / m1; wn[127] = tn / m1;
        fn[0]=1.; fn[127] = exp(-.5 * tn * tn);		
        for (uint i = 126; i > 0; i--) {
            const double dn = sqrt(-2 * log(vn / tn + exp(-.5 * tn * tn)));
            kn[i + 1] = ULONG32((dn / tn) * m1);
            fn[i] = exp(-.5 * dn * dn);                
            wn[i] = dn / m1;
            tn = dn;
        }

        // Set up tables for REXP
        const double m2 = 4294967296.0; // 2^32
        const double ve = 3.949659822581572e-3;
        double te = 7.697117470131487;
        q = ve / exp(-te);
        ke[0] = ULONG32((te / q) * m2); ke[1] = 0;
        we[0] = q / m2; we[255] = te / m2;
        fe[0] = 1.; fe[255] = exp(-te);		
        for (uint i = 254; i > 0; i--) {
            const double de = -log(ve / te + exp(-te));
            ke[i+1] = ULONG32((de / te) * m2);
            fe[i] = exp(-de);
            we[i] = de / m2;
            te = de;
        }
    }

    void init()
        { tm = 1234567; z = w = jsr = jcong = ulong(time(0)) + tm; tm += 123457; }
    void init(ulong rank)
        { tm = 1234567; z = w = jsr = jcong = ulong(time(0)) + tm/rank; tm += 123457; }
    void init(ulong z_, ulong w_, ulong jsr_, ulong jcong_ )
        { tm = 1234567; z = z_; w = w_; jsr = jsr_; jcong = jcong_; }

    // For a faster but lower quality RNG, uncomment the following
    // line, and comment out the original definition of rand_int above.
    // In practice, the faster RNG will be fine for simulations
    // that do not simulate more than a few billion random numbers.
    // ulong rand_int() { return SHR3(); }
    long rand_int31()                    // [0,2^31-1]
        { return ((long) rand_int32() >> 1);}
    double rand_closed01()         // [0,1]
        { return ((double) rand_int() / double(ULONG_MAX)); }
    double rand_open01()             // (0,1)
        { return (((double) rand_int() + 1.0) / (ULONG_MAX + 2.0)); }
    double rand_halfclosed01() // [0,1)
        { return ((double) rand_int() / (ULONG_MAX + 1.0)); }
    double rand_halfopen01()     // (0,1]
        { return (((double) rand_int() + 1.0) / (ULONG_MAX + 1.0)); }

    // Continuous Distributions
    double uniform(double x = 0.0, double y = 1.0)
        { return rand_closed01() * (y - x) + x; }
    double normal(double mu = 0.0, double sd = 1.0)
        { return RNOR() * sd + mu; }
    double exponential(double lambda = 1)
        { return REXP() / lambda; }
    double gamma(double shape = 1, double scale = 1) {
        if (shape < 1.)
        return gamma(shape + 1., scale) * pow(rand_open01(), 1.0 / shape);

        const double d = shape - 1. / 3.;
        const double c = 1. / sqrt(9. * d);
        double x, v;
        for (;;) {
            do {
                x = RNOR();
                v = 1.0 + c * x;
            } while (v <= 0.0);
            v = v * v * v;
            const double u = rand_open01();
            const double x2 = x * x;
            if (u < 1.0 - 0.0331 * x2 * x2)
                return (d * v / scale);
            if (log(u) < 0.5 * x2 + d * (1.0 - v + log(v)))
                return (d * v / scale);
        }
    }
    double chi_square(double df)
        { return gamma(df / 2.0, 0.5); }
    double beta(double a1, double a2)
        { const double x1 = gamma(a1, 1); return (x1 / (x1 + gamma(a2, 1))); }

    void uniform(vector<double>& res, double x = 0.0, double y = 1.0) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = uniform(x, y);
    }
    void normal(vector<double>& res, double mu = 0.0, double sd = 1.0) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = normal(mu, sd);
    }
    void exponential(vector<double>& res, double lambda = 1) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = exponential(lambda);
    }
    void gamma(vector<double>& res, double shape = 1, double scale = 1) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = gamma(shape, scale);
    }
    void chi_square(vector<double>& res, double df) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = chi_square(df);
    }
    void beta(vector<double>& res, double a1, double a2) {
        for (vector<double>::iterator i = res.begin(); i != res.end(); ++i)
            *i = beta(a1, a2);
    }

    // Discrete Distributions
    unsigned int random(unsigned int n) {
        return (unsigned int) ( n * uniform() );
    }
    unsigned int random(unsigned int min, unsigned int max) {
        return (unsigned int) ( (max + 1 - min) * uniform() + min);
    }
    int poisson(double mu) {
        const double a0=-0.5, a1= 0.3333333, a2=-0.2500068, a3= 0.2000118,
	a4=-0.1661269, a5= 0.1421878, a6=-0.1384794, a7= 0.1250060;

        const double one_7 = 1.0 / 7.0, one_12 = 1.0 / 12.0, one_24 = 1.0 / 24.0;

        const double fact[10] =
        { 1., 1., 2., 6., 24., 120., 720., 5040., 40320., 362880.    };

        static int l, m;

        static double b1, b2, c, c0, c1, c2, c3;
        static double pp[36], p0, p, q, s, d, omega;
        static double big_l;/* integer "w/o overflow" */
        static double muprev = 0., muprev2 = 0.;/*, muold         = 0.*/

        double del, difmuk= 0., E= 0., fk= 0., fx, fy, g, px, py, t, u= 0., v, x;
        int pois = -1;
        int k, kflag, big_mu, new_big_mu = 0;

        if (mu <= 0.)
            return 0;

        big_mu = mu >= 10.;
        if(big_mu)
            new_big_mu = 0;

        if (!(big_mu && mu == muprev)) {

            if (big_mu) {
                new_big_mu = 1;
                muprev = mu;
                s = sqrt(mu);
                d = 6. * mu * mu;
                big_l = floor(mu - 1.1484);
            }
            else {
                if (mu != muprev) {
            muprev = mu;
            m = (int) ((1.0 < mu) ? mu : 1.0);
            l = 0; /* pp[] is already ok up to pp[l] */
            q = p0 = p = exp(-mu);
                }

                for (;;) {
            u = rand_open01();
            if (u <= p0)
                return 0;

            if (l != 0) {
                for (k = (u <= 0.458) ? 1 : ((l < m) ? l : m);    k <= l; k++)
                    if (u <= pp[k])
                        return k;
                if (l == 35) /* u > pp[35] */
                    continue;
            }
            l++;
            for (k = l; k <= 35; k++) {
                p *= mu / k;
                q += p;
                pp[k] = q;
                if (u <= q) {
                    l = k;
                    return k;
                }
            }
            l = 35;
                }
            }

        }

        g = mu + s * RNOR();
        if (g >= 0.) {
            pois = int(g);
            if (pois >= big_l)
                return pois;
            fk = pois;
            difmuk = mu - fk;
            u = rand_open01();
            if (d * u >= difmuk * difmuk * difmuk)
                return pois;
        }

        if (new_big_mu || mu != muprev2) {
            muprev2 = mu;
            omega = 1.0 / (sqrt(2.0 * PI) * s);
            b1 = one_24 / mu;
            b2 = 0.3 * b1 * b1;
            c3 = one_7 * b1 * b2;
            c2 = b2 - 15. * c3;
            c1 = b1 - 6. * b2 + 45. * c3;
            c0 = 1. - b1 + 3. * b2 - 15. * c3;
            c = 0.1069 / mu;
        }

        if (g >= 0.) {
            kflag = 0;
            goto Step_F;
        }


        for (;;) {
            E = REXP();
            u = 2 * rand_open01() - 1.;
            t = 1.8 + ((u > 0) ? std::abs(E) : -std::abs(E));
            if (t > -0.6744) {
                pois = int(mu + s * t);
                fk = pois;
                difmuk = mu - fk;
                kflag = 1;
    Step_F:
                if (pois < 10) {
            px = -mu;
            py = pow(mu, pois) / fact[pois];
                }
                else {
            del = one_12 / fk;
            del = del * (1. - 4.8 * del * del);
            v = difmuk / fk;
            if (std::abs(v) <= 0.25)
                px = fk * v * v * (((((((a7 * v + a6) * v + a5) * v + a4) *
                            v + a3) * v + a2) * v + a1) * v + a0)
                    - del;
            else
                px = fk * log(1. + v) - difmuk - del;
            py = 1.0 / (sqrt(2.0 * PI) * sqrt(fk));
                }
                x = (0.5 - difmuk) / s;
                x *= x;/* x^2 */
                fx = -0.5 * x;
                fy = omega * (((c3 * x + c2) * x + c1) * x + c0);
                if (kflag > 0) {
            if (c * std::abs(u) <= py * exp(px + E) - fy * exp(fx + E))
                break;
                } else
            if (fy - u * fy <= py * exp(px - fx))
                break;
            }
        }
        return pois;
    }
    int binomial(double prob, int n) {
        static double c, fm, npq, p1, p2, p3, p4, qn, xl, xll, xlr, xm, xr;

        static double psave = -1.0;
        static int nsave = -1, m = 0;

        double f, x;

        if (n <= 0 || prob <= 0.) return 0;
        if (prob >= 1.) return n;

        const double p = (prob < 1. - prob) ? prob : 1. - prob;
        const double q = 1. - p;
        const double np = n * p;
        const double r = p / q;
        const double g = r * (n + 1);

        if (prob != psave || n != nsave) {
            psave = prob;
            nsave = n;
            if (np < 30.0) {
                qn = pow(q, (double) n);
                goto L_np_small;
            } else {
                double ffm = np + p;
                m = (int) ffm;
                fm = m;
                npq = np * q;
                p1 = (int)(2.195 * sqrt(npq) - 4.6 * q) + 0.5;
                xm = fm + 0.5;
                xl = xm - p1;
                xr = xm + p1;
                c = 0.134 + 20.5 / (15.3 + fm);
                double al = (ffm - xl) / (ffm - xl * p);
                xll = al * (1.0 + 0.5 * al);
                al = (xr - ffm) / (xr * q);
                xlr = al * (1.0 + 0.5 * al);
                p2 = p1 * (1.0 + c + c);
                p3 = p2 + c / xll;
                p4 = p3 + c / xlr;
            }
        } else if (n == nsave) {
            if (np < 30.0)
                goto L_np_small;
        }

        int i, ix;
        for (;;) {
            double u = rand_open01() * p4;
            double v = rand_open01();
            if (u <= p1) {
                ix = (int) (xm - p1 * v + u);
                goto finish;
            }
            if (u <= p2) {
                x = xl + (u - p1) / c;
                v = v * c + 1.0 - std::abs(xm - x) / p1;
                if (v > 1.0 || v <= 0.)
            continue;
                ix = (int) x;
            } else {
                if (u > p3) {
            ix = (int) (xr - log(v) / xlr);
            if (ix > n)
                continue;
            v *= (u - p3) * xlr;
                } else {/* left tail */
            ix = (int) (xl + log(v) / xll);
            if (ix < 0)
                continue;
            v *= (u - p2) * xll;
                }
            }
            int k = std::abs(ix - m);
            if (k <= 20 || k >= npq / 2 - 1) {
                f = 1.0;
                if (m < ix) {
            for (i = m + 1; i <= ix; i++)
                f *= (g / i - r);
                } else if (m != ix) {
            for (i = ix + 1; i <= m; i++)
                f /= (g / i - r);
                }
                if (v <= f)
            goto finish;
            } else {
                double amaxp, ynorm, alv;
                amaxp = (k / npq) * ((k * (k / 3. + 0.625) + 0.1666666666666) /
                npq + 0.5);
                ynorm = -k * k / (2.0 * npq);
                alv = log(v);
                if (alv < ynorm - amaxp)
            goto finish;
                if (alv <= ynorm + amaxp) {
            double x1, f1, z, w, z2, x2, f2, w2;
            x1 = ix + 1;
            f1 = fm + 1.0;
            z = n + 1 - fm;
            w = n - ix + 1.0;
            z2 = z * z;
            x2 = x1 * x1;
            f2 = f1 * f1;
            w2 = w * w;
            if (alv <= xm * log(f1 / x1) + (n - m + 0.5) * log(z / w) +
                    (ix - m) * log(w * p / (x1 * q)) + (13860.0 - (462.0 -
                    (132.0 - (99.0 - 140.0 / f2) / f2) / f2) / f2) / f1 / 166320.0 +
                    (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / z2) / z2) / z2) / z2) /
                    z / 166320.0 + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / x2) /
                        x2) / x2) / x2) / x1 / 166320.0 + (13860.0 - (462.0 - (132.0 -
                            (99.0 - 140.0 / w2) / w2) / w2) / w2) / w / 166320.)
                goto finish;
                }
            }
        }

    L_np_small:
        for (;;) {
            ix = 0;
            f = qn;
            double u = rand_open01();
            for (;;) {
                if (u < f)
            goto finish;
                if (ix > 110)
            break;
                u -= f;
                ix++;
                f *= (g / ix - r);
            }
        }

    finish:
        if (psave > 0.5)
            ix = n - ix;
        return ix;

    }
    void multinom(unsigned int n, const vector<double>& probs, vector<uint>& samp) 
    {
        samp.resize(probs.size());
        edaRNG::multinom(n, &probs[0], (uint) probs.size(), &samp[0]);
    }
    void multinom(unsigned int size, const double* probs, uint num_probs, uint* samp) {
        if (num_probs == 0) return;
        for (uint i = 0; i < num_probs; i++) samp[i] = 0;
        if (size == 0) return;

        vector<double> fixed_probs(num_probs);
        double total_prob = 0.;
        for (uint i = 0; i < num_probs; i++) {
            const double pp = probs[i];
            //if (std::isfinite(pp) && pp >= 0)
            if ((pp == pp) && pp >= 0)
                total_prob += (fixed_probs[i] = pp);
        }

        if (total_prob == 0.) return;

        for (uint i = 0; i < num_probs-1; i++) {
            if (fixed_probs[i] > 0.) {
                samp[i] = binomial(fixed_probs[i] / total_prob, size);
                size -= samp[i];
            }
            if (size == 0) return;
            total_prob -= fixed_probs[i];
        }
        samp[num_probs - 1] = size;
    }

    void poisson(vector<int>& res, double lambda) {
        for (vector<int>::iterator i = res.begin(); i != res.end(); ++i)
            *i = poisson(lambda);
    }
    void binomial(vector<int>& res, double p, int n) {
        for (vector<int>::iterator i = res.begin(); i != res.end(); ++i)
            *i = binomial(p, n);
    }
    const char* className() const {
        return "edaRNG";
    }
}; 
#endif /* EDARNG_H */

