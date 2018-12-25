#include<RcppEigen.h>
#include<iostream>
#include<vector>
#include <trng/yarn2.hpp>
#include <trng/discrete_dist.hpp>
// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;
using namespace std;

SparseMatrix<double,ColMajor> RandomSample(MatrixXd A,int n){
  MatrixXd prob=0.5*(A.cwiseProduct(A)/A.norm()+A.cwiseAbs()/(A.colwise().lpNorm<1>().maxCoeff()));
  
  vector<double> probvec(prob.cols()*prob.rows());
  int i=0,j=0;
  for(;j<prob.cols();j++)
    for(;i<prob.rows();i++)
      probvec.at(j*prob.cols()+i)=prob(i,j);
  trng::yarn2 rx;
  rx.seed(10);
  trng::discrete_dist distSampling(probvec.begin(), probvec.end());
  int ix=0,iy=0;
  int r;
  typedef Eigen::Triplet<double> T;
  std::vector<T> tripletList;
  tripletList.reserve(n);
  for(i=0;i<=n;i++)
  {
    r=distSampling(rx);
    ix=r%prob.cols();
    iy=r/prob.cols();
    tripletList.push_back(T(ix,iy,A(ix,iy)/prob(ix,iy)));
  }
  SparseMatrix<double,ColMajor> mat(A.rows(),A.cols());
  mat.setFromTriplets(tripletList.begin(), tripletList.end());
  mat/=n;
  mat.makeCompressed();
  return mat;

}

// [[Rcpp::export]]
Rcpp::List SparseEigenPowerIteration(MatrixXd B,int times=1000,int n1=1000){
  //https://eigen.tuxfamily.org/dox/classEigen_1_1MatrixBase.html#a72586ab059e889e7d2894ff227747e35
  SparseMatrix<double,ColMajor> A=RandomSample(B,n1);
  int m=B.rows(),n=B.cols();
  SparseVector<double,ColMajor> x0(n);
  x0.insert(1,0)=1;
  x0.insert(2,0)=2;
  for(int i=0;i<times;i++){
    x0=A.transpose()*A*x0;
    //avoid overflow;
    x0/=x0.norm();
  }
  //
  
  SparseVector<double,ColMajor> v1=x0;
  double sigma1=(A*v1).norm();
  SparseVector<double,ColMajor> u1=A*v1/sigma1;
  return Rcpp::List::create(Rcpp::Named("Sigma1")=Rcpp::wrap(sigma1),
                            Rcpp::Named("U1")=Rcpp::wrap(u1),
                            Rcpp::Named("V1")=Rcpp::wrap(v1));
}
/*** R
Sys.setenv("PKG_LIBS"="-ltrng4")
# read in image
library(jpeg)
img = readJPEG("./dog.jpg")
img = img[,,1]
s <- SparseEigenPowerIteration(img)
s$Sigma1
*/
