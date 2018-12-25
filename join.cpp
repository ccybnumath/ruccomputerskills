#include <RcppArmadillo.h>
#include <RcppArmadilloExtensions/sample.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;


// [[Rcpp::export]]
mat join(){
  mat A = randu<mat>(4,5);
  mat B = randu<mat>(4,6);
  mat C = randu<mat>(6,5);
  
  mat X = join_rows(A,B);
  mat Y = join_cols(A,C);
  return Y;

}

