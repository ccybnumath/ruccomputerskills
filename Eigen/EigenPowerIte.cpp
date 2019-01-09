#include<RcppEigen.h>
#include<iostream>
// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;
using namespace std;

// [[Rcpp::export]]
Rcpp::List EigenPowerIteration(MatrixXd A,int times=1000){
  int m=A.rows(),n=A.cols();
  VectorXd x0=VectorXd::Ones(n,1);
  
  for(int i=0;i<times;i++){
    x0=A.transpose()*A*x0;
    //avoid overflow;
    x0.normalize();
  }
  //
  
  VectorXd v1=x0;
  double sigma1=(A*v1).norm();
  VectorXd u1=A*v1/sigma1;
  return Rcpp::List::create(Rcpp::Named("Sigma1")=Rcpp::wrap(sigma1),
                            Rcpp::Named("U1")=Rcpp::wrap(u1),
                            Rcpp::Named("V1")=Rcpp::wrap(v1));
}
/*** R
# read in image
library(jpeg)
img = readJPEG("./dog.jpg")
img = img[,,1]
s <- EigenPowerIteration(img)
s$Sigma1
*/