#include <RcppArmadillo.h>
#include <omp.h>

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

using namespace arma;
using namespace std;

// [[Rcpp::export]]
mat squaremat1(){
  // for matrices with real elements
  mat A = randu<mat>(500,500);
  mat B = A.t()*A;  // generate a symmetric matrix
  vec eigval;
  mat eigvec;
  eig_sym(eigval, eigvec, B);
  
  // method one
  mat squareRoot1;
  squareRoot1 = eigvec * diagmat(sqrt(eigval)) * eigvec.t();
  return squareRoot1;
}

// [[Rcpp::export]]
mat squaremat2(){
  // for matrices with real elements
  mat A = randu<mat>(500,500);
  mat B = A.t()*A;  // generate a symmetric matrix
  vec eigval;
  mat eigvec;
  eig_sym(eigval, eigvec, B);
  
  // method two
  mat squareRoot2;
  eigval = pow(eigval, 0.25);
  eigvec.each_row() %= eigval.t();
  squareRoot2 = eigvec * eigvec.t();
  
  return squareRoot2;
}

/*** R
library(microbenchmark)
microbenchmark(squaremat1(),squaremat2())
*/