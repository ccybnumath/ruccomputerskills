#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]




// [[Rcpp::export]]
vec addTwo_parallel(vec x, vec y){
  vec z(x.n_elem);
  int i;
#pragma omp parallel for schedule(static) \
  private(i) shared(x,y,z)
    for(i=0;i<x.n_elem;i++)
      z.at(i)=x.at(i)+y.at(i);
  return z;
}

// [[Rcpp::export]]
vec addTwo_serial(vec x, vec y){
  vec z(x.n_elem);
  int i;
  for(i=0; i < x.n_elem; i++)
    z(i) = x(i) + y(i);
  return z;
}

// [[Rcpp::export]]
vec addTwo_direct(vec x, vec y){
  return x + y;
}


/*** R
library(microbenchmark)
n = 1e8
x=runif(n)
y=runif(n)
res = microbenchmark(addTwo_serial(x,y),addTwo_parallel(x,y),addTwo_direct(x,y),times = 20)
print(res)
*/