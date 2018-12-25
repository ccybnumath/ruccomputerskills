#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]




// [[Rcpp::export]]
double vectorNorm_parallel(vec x){
  int i;
  double sum=0;
#pragma omp parallel for reduction(+: sum)\
  private(i) shared(x) schedule(static)
    for(i=0;i<x.n_elem;i++)
      sum+=x(i)*x(i);
  sum = sqrt(sum);
  return sum;
}

// [[Rcpp::export]]
double vectorNorm_serial(vec x){
  int i;
  double sum = 0;
  for(i=0; i < x.n_elem; i++)
    sum += x(i)*x(i);
  sum = sqrt(sum);
  return sum;
}

// [[Rcpp::export]]
double vectorNorm_direct(vec x){
  return norm(x);
}