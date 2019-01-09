#include<RcppEigen.h>

// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;

// [[Rcpp::export]]
VectorXd getEigenValues(Map<MatrixXd> M){
  SelfAdjointEigenSolver<MatrixXd> es(M);
  MatrixXd eigVectors=es.eigenvectors();
  return es.eigenvalues();
}