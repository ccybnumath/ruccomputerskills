#include <RcppArmadillo.h>
#include <omp.h>
using namespace arma;

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export()]]
double calIntegration_serial(){
  double integral = 0;
  int i;
  int N=1e6;
  double xi;
  double dx=1.0/N;
  for(i=0;i<=N;i++){
    xi=exp(log(i)-log(N));
    integral+=4.0*dx/(1+xi*xi);
  }
  return integral;
}
// [[Rcpp::export()]]
double calIntegration_parallel(){
  double integral = 0;
  int i;
  int N=1e6;
  double xi;
  double dx=1.0/N;
#pragma omp parallel for reduction(+:integral)\
  private(i,xi) shared(N,dx) schedule(static)
  for(i=0;i<=N;i++){ 
    xi=exp(log(i)-log(N));
    integral+=4.0*dx/(1+xi*xi);
  }
  return integral;
}
/*** R
calIntegration_parallel()
x = microbenchmark(calIntegration_serial(),calIntegration_parallel())
*/