#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
mat rngCpp(const int N){
  mat X(N,4);
  //Uniform[-1,1]
  X.col(0)=vec(Rcpp::runif(N,-1,1));
  X.col(1)=vec(Rcpp::rnorm(N,0,10));
  X.col(2)=vec(Rcpp::rt(N,5));
  X.col(3)=vec(Rcpp::rbeta(N,1,1));
  return X;
}
/***R
rngCpp(5)
  */