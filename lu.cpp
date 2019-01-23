#include <RcppArmadillo.h>
#include <omp.h>

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

using namespace arma;
using namespace std;

// [[Rcpp::export]]
vec pivotLU(mat A, vec b, vec x0){
  vec xhat, error(2);
  mat L,U,P;
  lu(L,U,P,A);
  xhat = solve(trimatl(L), P*b);
  xhat = solve(trimatu(U), xhat);
  
  error(0) = norm(xhat - x0);
  error(1) = norm(A*xhat - b);
  return error;
}

// [[Rcpp::export]]
vec solveChol(mat A, vec b, vec x0){
  vec xhat, error(2);
  mat R=chol(A);
  xhat = solve(trimatl(R), b);
  xhat = solve(trimatu(R), xhat);
  
  error(0) = norm(xhat - x0);
  error(1) = norm(A*xhat - b);
  return error;
}

// [[Rcpp::export]]
vec solveDirect(mat A,vec b,vec x0){
  // Solve Ax=b, the true x0 is provided
  vec xhat,error(2);
  xhat=solve(A,b);
  error(0) = norm(xhat - x0);
  error(1) = norm(A*xhat - b);
  return error;
  
}

/*** R
library(Matrix)
n <- 5
A <- matrix(runif(n*n),nrow=5)
eigen(A)$value

x0 = rnorm(n)

b = A %*% x0
library(microbenchmark)
microbenchmark(solveDirect(A,b,x0),pivotLU(A, b, x0),solveChol(A,b,x0))

*/