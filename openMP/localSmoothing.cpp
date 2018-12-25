#include <RcppArmadillo.h>
#include <omp.h>
using namespace arma;

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

inline int mirrorIndex(int fetchI, int length){
    if(fetchI < 0)
        fetchI = -fetchI - 1;
    if(fetchI >= length){
        fetchI = length - (fetchI - length) - 1;
    }
    return fetchI;
}

// [[Rcpp::export]]
vec localSmoothing_serial(vec y, vec weights){
    int i, j, ySize = y.n_elem;
    vec smoothedY(ySize);
    double s;
    int fetchI;
    for(i = 0; i < ySize; i++){
        s = 0;
        for(j = -4; j < 5; j++){
            fetchI = i + j;
            fetchI = mirrorIndex(fetchI, ySize);
            s += weights(j+4) * y(fetchI);
        }
        smoothedY(i) = s;
    }
    
    return smoothedY;
}

// [[Rcpp::export]]
vec localSmoothing_parallel(vec y, vec weights){
    // write your parallel code here
    int i, j, ySize = y.n_elem;
  vec smoothedY(ySize);
  double s;
  int fetchI;
#pragma omp parallel for schedule(static) shared(ySize) private(i,j,s,fetchI) 
  for(i = 0; i < ySize; i++){
    s = 0;
    for(j = -4; j < 5; j++){
      fetchI = i + j;
      fetchI = mirrorIndex(fetchI, ySize);
      s += weights(j+4) * y(fetchI);
    }
    smoothedY(i) = s;
  }
  
  return smoothedY;
}

