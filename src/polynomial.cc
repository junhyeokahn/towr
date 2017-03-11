/**
@file   polynomial.cc
@author Alexander W. Winkler
@date   29.07.2016

@brief  A virtual class spliner with ready to use derived spliners

Spliners ready to use:
        - Linear Spliner
        - Cubic Spliner
        - Quintic Spliner
*/

#include <xpp/opt/polynomial.h>

namespace xpp {
namespace utils {

constexpr std::array<Polynomial::PolynomialCoeff, 6> Polynomial::AllSplineCoeff;

/**
 * The spliner always calculates the splines in the same way, but if the
 * spline coefficients are zero (as set by @ref Spliner()), the higher-order
 * terms have no effect
 */
bool Polynomial::GetPoint(const double dt, StateLin1d& out) const
{
  // sanity checks
  if (dt < -1e-10)
    throw std::runtime_error("spliner.cc called with dt<0");

  double dt1 = (dt > duration) ? duration : dt;
  double dt2 = dt1 * dt1;
  double dt3 = dt1 * dt2;
  double dt4 = dt1 * dt3;
  double dt5 = dt1 * dt4;

  out.p   = c[F] + c[E]*dt1 +   c[D]*dt2 +   c[C]*dt3 +    c[B]*dt4 +    c[A]*dt5;
  out.v  =        c[E]     + 2*c[D]*dt1 + 3*c[C]*dt2 +  4*c[B]*dt3 +  5*c[A]*dt4;
  out.a =                   2*c[D]     + 6*c[C]*dt1 + 12*c[B]*dt2 + 20*c[A]*dt3;
  out.j=                                6*c[C]     + 24*c[B]*dt1 + 60*c[A]*dt2;

  return true;
}

void LinearPolynomial::SetPolynomialCoefficients(double T, const StateLin1d& start, const StateLin1d& end)
{
  c[F] = start.p;
  c[E] = (end.p - start.p) / T;

  c[D] = c[C] = c[B] = c[A] = 0.0;
}

void CubicPolynomial::SetPolynomialCoefficients(double T, const StateLin1d& start, const StateLin1d& end)
{
  double T1 = T;
  double T2 = T1 * T1;
  double T3 = T1 * T2;

  c[F] = start.p;
  c[E] = start.v;
  c[D] = - ((3 * c[F]) - (3 * end.p) + (2 * T1 * c[E]) + (T1 * end.v)) / T2;
  c[C] =   ((2 * c[F]) - (2 * end.p) +      T1 * (c[E] +       end.v ))/ T3;

  c[B] = c[A] = 0.0;
}

void QuinticPolynomial::SetPolynomialCoefficients(double T, const StateLin1d& start, const StateLin1d& end)
{
  double T1 = T;
  double T2 = T1 * T1;
  double T3 = T1 * T2;
  double T4 = T1 * T3;
  double T5 = T1 * T4;

  c[F] = start.p;
  c[E] = start.v;
  c[D] = start.a / 2.;
  c[C] =  (-20*start.p + 20*end.p + T1*(-3*start.a*T1 +   end.a*T1 - 12* start.v -  8*end.v))  / (2*T3);
  c[B] =  ( 30*start.p - 30*end.p + T1*( 3*start.a*T1 - 2*end.a*T1 + 16* start.v + 14*end.v))  / (2*T4);
  c[A] = -( 12*start.p - 12*end.p + T1*(   start.a*T1 -   end.a*T1 +  6*(start.v +    end.v))) / (2*T5);
}

} // namespace utils
} // namespace xpp