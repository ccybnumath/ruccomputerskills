#include<RcppEigen.h>
#include<iostream>
#include<vector>
using namespace Eigen;
using namespace std;

// [[Rcpp::depends(RcppEigen)]]



typedef Eigen::Triplet<double> T;
typedef SparseMatrix<double> SpMat;
// [[Rcpp::export]]
SpMat createLmat(int N){
  std::vector<T> tripletList;
  int i,j;
  double v_ij;
  tripletList.reserve(N);
  for(i=0;i<N;i++)
    for(j=0;j<N;j++)
  {
    v_ij=i;
    tripletList.push_back(T(i,j,v_ij));
  }
  SpMat mat(N,N);
  mat.setFromTriplets(tripletList.begin(), tripletList.end());
  return mat;
}