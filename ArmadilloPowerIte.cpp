#include <RcppArmadillo.h>
#include <iostream>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;
using namespace std;

// [[Rcpp::export]]
Rcpp::List ArmaPowerIteration(mat A,int times=1000){
  int m=A.n_rows,n=A.n_cols;
  vec x0=ones<vec>(n);
  
  for(int i=0;i<times;i++){
    x0=A.t()*A*x0;
    //avoid overflow;
    x0=x0/norm(x0,2);
  }
  //
  vec v1=x0/norm(x0,2);
  double sigma1=norm(A*v1,2);
  vec u1=A*v1/sigma1;
  return Rcpp::List::create(Rcpp::Named("Sigma1")=Rcpp::wrap(sigma1),
                            Rcpp::Named("U1")=Rcpp::wrap(u1),
                            Rcpp::Named("V1")=Rcpp::wrap(v1));
  
}
/*** R
# read in image
library(jpeg)
img = readJPEG("./dog.jpg")
img = img[,,1]
s <- ArmaPowerIteration(img)
s$Sigma1
*/