#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
Rcpp::List fastLM(vec y,mat X){
  vec betaHat,betaSigma,XtY,residuals;
  mat XtX;
  double sigmaHat;
  
  int nSample=X.n_rows;
  int pCov=X.n_cols;
  
  // equivalent to betaHat=solve(X,y);
  XtX=X.t()*X;
  XtY=X.t()*y;
  //XtX*betaHat=XtY
  betaHat=solve(XtX,XtY);
  
  residuals=y-X*betaHat;
  sigmaHat=sum(residuals%residuals)/(nSample-pCov);
  sigmaHat=sqrt(sigmaHat);
  
  // Inverse: XtX.i(), solve(XtX), or inv(XtX)
  betaSigma = sigmaHat * sqrt(diagvec(XtX.i()));
  
  return Rcpp::List::create(Rcpp::Named("betaHat")=betaHat,
                            Rcpp::Named("betaSigma")=betaSigma,
                            Rcpp::Named("sigmaHat")=sigmaHat);
}