#include <RcppEigen.h>

// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;

// [[Rcpp::export]]
void modifyV1(MatrixXd x) {
  x(0,0) =  1;
}

// [[Rcpp::export]]
void modifyV2(Map<MatrixXd> x) {
  x(0,0) =  1;
}
/*** R
A <- matrix(c(0,0,0,0),nrow=2)
modifyV1(A)
modifyV2(A)
*/
