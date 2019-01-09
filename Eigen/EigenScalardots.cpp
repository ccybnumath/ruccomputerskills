#include<RcppEigen.h>
#include<iostream>
// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;
using namespace std;


// [[Rcpp::export]]
MatrixXd scalardots(){
  MatrixXd A=MatrixXd::Ones(2,2);
  A*=-1;
  return A;
}
