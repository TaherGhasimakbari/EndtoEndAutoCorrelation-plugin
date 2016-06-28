#ifndef UTIL_PRODUCT_H
#define UTIL_PRODUCT_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include <complex>
#include <hoomd/hoomd.h>
using std::complex;

namespace Util
{

   /**
   * Product for float Data.
   */
   inline float product(float a, float b)
   {  return a*b; }

   /**
   * Product for double Data.
   */
   inline double product(double a, double b)
   {  return a*b; }

   /**
   * Dot product for vector Data.
   */
   inline double product(Scalar3 a, Scalar3 b)
   {
      return a.x*b.x+a.y*b.y+a.z*b.z;
   }

   /**
   * Inner product for complex<float> Data.
   */
   inline complex<float> product(complex<float> a, complex<float> b)
   {  return conj(a)*b; }

   /**
   * Inner product for complex<double> Data.
   */
   inline complex<double> product(complex<double> a, complex<double> b)
   {  return conj(a)*b; }

}
#endif
