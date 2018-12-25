#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
vec pivotLU(mat A,vec b,vec x0){
  vec xhat,error(2);
  mat L,U,P;
  lu(L,U,P,A);
  xhat=solve(trimatl(L),P*b);
  xhat=sovle(trimatu(U),xhat);
  
  error(0)=norm(xhat-x0);
  error(1)=norm(A*xhat-b);
  return error;
}

// [[Rcpp::export]]
vec pivotLU(mat A,vec b,vec x0){
  vec xhat,error(2);
  // directly solve the linear system
  vec xhat = solve(A,b);
  error(0) = norm(xhat - x0);
  error(1) = norm(A*xhat - b);
}

/***R
options(digits = 3)
set.seed(100)
library(Matrix)
n=7
A=as.matrix(Hilbert(n))
eigen(A)$value
x0=rnorm(n)
b=A%*%x0
pivotLU(A,b,x0)
*/