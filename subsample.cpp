#include <RcppArmadillo.h>
#include <RcppArmadilloExtensions/sample.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;


// [[Rcpp::export]]
vec linearRegression01(vec y,mat X,int q){
  vec beta;
  mat XSub;
  vec ySub;
  
  uvec fullvec;
  fullvec=regspace<uvec>(0,y.n_elem-1);
  
  uvec sampleIndex=Rcpp::RcppArmadillo::sample(fullvec,q,true);
  
  XSub=X.rows(sampleIndex);
  ySub=y.rows(sampleIndex);
  beta=solve(XSub.t()*XSub,XSub.t()*ySub);
  
  return beta;
}

// [[Rcpp::export]]
vec linearRegression02(vec y,mat X,int q){
  vec beta;
  mat XSub,HatMat;
  vec ySub,infScore;
  
  uvec fullvec;
  fullvec=regspace<uvec>(0,y.n_elem-1);
  
  //Compute leverage score
  HatMat=X*solve(X.t()*X,X.t());
  infScore=diagvec(HatMat);
  infScore/=sum(infScore);
  
  //Sampling by the leverage score
  uvec sampleIndex=Rcpp::RcppArmadillo::sample(fullvec,q,true,infScore);
  
  XSub=X.rows(sampleIndex);
  ySub=y.rows(sampleIndex);
  beta=solve(XSub.t()*XSub,XSub.t()*ySub);
  
  return beta;
}

/***R
set.seed(10)
n = 1e4
p = 5
sigma_e = 0.1
(beta = rnorm(p, sd = 5))
X = matrix(rnorm(n*p), n, p)
y = X %*% beta + rnorm(n, sd = sigma_e)
  
betaHat01 = linearRegression01(y, X, 2e3)
betaHat02 = linearRegression02(y, X, 2e3)
sum(abs(betaHat01 - beta))
# 0.008758408
sum(abs(betaHat02 - beta))
# 0.002565776
*/