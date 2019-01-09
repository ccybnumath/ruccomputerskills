#include <RcppArmadillo.h>
#include <RcppArmadilloExtensions/sample.h>
using namespace arma;
// [[Rcpp::depends(RcppArmadillo)]]


void hadamardTransform(double * x, int startI, int endI){
  if(endI == startI + 1)
    return;
  int midI, j, length;
  double tmp1, tmp2;
  length = (endI - startI) / 2;
  midI = length + startI;
  hadamardTransform(x, startI, midI);
  hadamardTransform(x, midI, endI);
  for(j = 0; j < length; j++){
    tmp1 = x[j + startI];
    tmp2 = x[j + midI];
    x[j + startI] = tmp1 + tmp2;
    x[j + midI] = tmp1 - tmp2;
  }
}


// [[Rcpp::export]]
mat THDTrans(mat & X, int m, double q){
  vec  dvec;
  dvec = Rcpp::runif(X.n_rows);
  X.rows(find(dvec < 0.5)) *= -1;
  double sign;
  
  int j;
  for(j = 0; j < X.n_cols; j++)
    hadamardTransform(X.colptr(j), 0, X.n_rows);
  
  // Full matrix
  // One-by-one, if slow
  int nextQ;
  X = X.t();
  mat result(X.n_rows, m,  fill::zeros);
  for(j = 0; j < m; j++){
    nextQ = R::rgeom(q);
    while(nextQ < X.n_cols){
      sign = R::runif(0.0, 1.0);
      sign = (sign > 0.5) - (sign <= 0.5);
      result.col(j) += sign * X.col(nextQ);
      nextQ += R::rgeom(q) + 1;
    }
  }
  
  return result.t();
}

// [[Rcpp::export]]
vec randomizedRegression(mat & Ab, int m, double q){
  mat DataSub, XSub, ySub;
  vec beta;
  DataSub = THDTrans(Ab, m, q);
  XSub = DataSub.cols(0, DataSub.n_cols - 2);
  ySub = DataSub.col(DataSub.n_cols - 1);
  beta = solve(XSub,  ySub); //solve(XSub.t() * XSub, XSub.t() * ySub);
  return beta;
}