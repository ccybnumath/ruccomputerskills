#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]



// [[Rcpp::export]]
vec sumvec_parallel1(mat Obs, uword p,uword n){
  uword i;

  vec globalSum(p,fill::zeros);
  
#pragma omp parallel shared(globalSum) private(i)
{
  vec localSum(p,fill::zeros);//local variable,thus is private
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
vec sumvec_parallel2(mat Obs, uword p,uword n){
  uword i;
  
  vec globalSum(p,fill::zeros);
  
#pragma omp parallel shared(globalSum) private(i)
{
  vec localSum(p,fill::zeros); //local variable,thus is private
#pragma omp for schedule(static)
  for(i=0;i<n;i++){
    localSum+=Obs.col(i);//why +=?think. ref:SimplePi.cpp #pragma omp for is magic order implementing that.
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
n <- 1000;
p <- 1000;
data <- as.matrix(Hilbert(n));
result <- sumvec_parallel1(data,p,n)-sumvec_parallel2(data,p,n)
sum(result)
library(microbenchmark)
microbenchmark(sumvec_parallel1(data,p,n),sumvec_parallel2(data,p,n))
  */
