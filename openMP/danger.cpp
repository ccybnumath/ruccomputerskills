#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]

// [[Rcpp::export]]

/*** R
library(microbenchmark)

*/