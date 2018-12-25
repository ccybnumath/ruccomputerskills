#include <RcppArmadillo.h>
#include <RcppArmadilloExtensions/sample.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;


// [[Rcpp::export]]
vec RidgeRegression(vec y,mat X,double lambda){
  vec yhat=zeros<mat>(y.n_elem,1);
  mat U,V;
  vec s;
  svd(U,s,V,X);
  for(int i=0;i<s.n_elem;i++){
    yhat+=sum(s(i)*s(i)/(s(i)*s(i)+lambda)*U.col(i).t()*y)*U.col(i);
  }
  return yhat;
}


/***R
set.seed(10)
n = 1e4
p = 5
sigma_e = 0.1
(beta = rnorm(p, sd = 5))
X = matrix(rnorm(n*p), n, p)
y = X %*% beta + rnorm(n, sd = sigma_e)

yhat=RidgeRegression(y,X,10)
sum(abs(y-yhat))

  */