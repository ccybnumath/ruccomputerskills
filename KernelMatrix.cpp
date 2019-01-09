#include <RcppArmadillo.h>
#include <omp.h>

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]

using namespace arma;
using namespace std;

double squaredExp(const vec & x1, 
                         const vec & x2){
  double val;
  val=norm(x1-x2);
  val=exp(-val*val/2.0);
  return val;
}

// [[Rcpp::export]]
mat computeCov(mat X){
  int N=X.n_cols;
  int i,j;
  // containers
  mat covMat(N,N);
  
  //
  for(i = 0; i<N; i++){
    for(j=0; j<N; j++){
        covMat(i,j) = squaredExp(X.col(i),X.col(j));
    }
  }
  
  return covMat;
}

// [[Rcpp::export]]

mat computeCov_parallel(mat X){
  int N=X.n_cols;
  int i,j;
  // containers
  mat covMat(N,N);
  
#pragma omp parallel for schedule(static) collapse(2)
  for(i = 0; i<N; i++){
    for(j=0; j<N; j++){
      covMat(i,j) = squaredExp(X.col(i),X.col(j));
    }
  }
  
  return covMat;
}

/*** R
p=4
n=1e4#对于大矩阵有问题
dataX=matrix(rnorm(p*n),nrow = p,ncol = n)
microbenchmark(res1 = computeCov(dataX),res2 = computeCov_parallel(dataX),times = 1)

*/
