#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]


// [[Rcpp::export]]
vec sumVec_parallel(){
  uword i,p,n;
  n = 1000;
  p = 100;
  mat Obs(p,n,fill::randu);
  vec globalSum(p,fill::zeros);
  
#pragma omp parallel shared(globalSum) private(i)
{ 
  vec localSum(p,fill::zeros);
#pragma omp for schedule(static)
  for(i=0;i<n;i++){
    localSum+=Obs.col(i);
  }
#pragma omp critical
  globalSum += localSum;
}
  return globalSum/=n;
}

// [[Rcpp::export]]
vec sumvec_parallel(mat Obs, uword p,uword n){
  uword i;

  vec globalSum(p,fill::zeros);
  
#pragma omp parallel shared(globalSum) private(i)
{
  vec localSum(p,fill::zeros);
#pragma omp for schedule(static)
  for(i=0;i<n;i++){
    localSum+=Obs.col(i);
  }
#pragma omp critical
  globalSum += localSum;
}
return globalSum/=n;
}

// [[Rcpp::export]]
vec sumvec_serial(mat Obs, uword p,uword n){
  return sum(Obs,1)/n;
}

/*** R
library(Matrix)
n <- 10000;
p <- 10000;
data <- as.matrix(Hilbert(n));
result <- sumvec_parallel(data,p,n)-sumvec_serial(data,p,n)
sum(result)
  */
