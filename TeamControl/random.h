#ifndef _random_h_
#define _random_h_

#include "Vec.h"
#include <cstdlib>
#include <cmath>

 
  inline void random_seed (const unsigned int& s) throw () {
     srand(s); }

  /** gleichverteilte Zufallszahl in [0,1] */
  inline double urandom () throw () {
    return static_cast<double>( rand())/RAND_MAX; }

  /** gleichverteilte Zufallszahl in [f,t] */
  inline double urandom (const double& f, const double& t) throw () {
    return (t-f)*urandom ()+f; }

  /** Bernoulli-Experiment mit Erfolgswahrscheinlichkeit p */
  inline bool brandom (const double& p) throw () {
    return (urandom()<p); }

  /** Standard-normalverteilte Zufallszahl */
  inline double nrandom () throw () {
    double u1 = urandom();
    double u2 = urandom();
    return ( sqrt(-2.0* log(u1))*cos(6.2831853071*u2));
  }

  /** Normalverteilte Zufallszahl N(mu,sigma^2) */
  inline double nrandom (const double& mu, const double& sigma) throw () {
    return nrandom()*sigma+mu; }

  /** Standard-Normalverteilter Zufallszahlvektor */
  inline Vec n2random () throw () {
    double u1 = urandom();
    double u2 = urandom();
    double s =  sqrt(-2.0* log(u1));
    return Vec (s*cos(6.2831853071*u2), s*sin(6.2831853071*u2));
  }

 

#endif
