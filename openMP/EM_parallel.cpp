#include <RcppArmadillo.h>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

double f (double mu) {
  double val = ((R::dnorm(-mu, 0, 1, false)) /
                (1 - R::pnorm(-mu, 0, 1, true, false))
  ) ;
  return (val) ;
}

double g (double mu) {
  double val = ((R::dnorm(-mu, 0, 1, false)) /
                (R::pnorm(-mu, 0, 1, true, false))
  ) ;
  return (val) ;
}