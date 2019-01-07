#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

#define CUTOFF 20000

// [[Rcpp::export]]
double computeSum_dc(const vec & data, size_t beginI, size_t endI){
  size_t n=endI-beginI;
  if(n==0){
    return 0;
  }
  else if(n==1){
    return data.at(beginI);
  }
  
  size_t half=n/2;
  
  double result;
  result=computeSum_dc(data,beginI,beginI+half);
  result+=computeSum_dc(data,beginI+half,endI);
  return result;
}

// [[Rcpp::export]]
double computeSum_dc_parallel(const vec & data, size_t beginI, size_t endI){
  size_t n=endI-beginI;
  if(n==0){
    return 0;
  }
  else if(n==1){
    return data.at(beginI);
  }
  
  size_t half=n/2;
    
  double r1,r2;
  //only one thread exect code below, shared do nothing.
#pragma omp parallel shared(data,beginI,endI,half,r1,r2)
{
  //nowait here does nothing.
#pragma omp single nowait
{
#pragma omp task shared(r1, data, beginI, half, endI)
  r1 = computeSum_dc(data, beginI, beginI + half);
#pragma omp task shared(r2, data, beginI, half, endI)
  r2 = computeSum_dc(data, beginI + half, endI);
#pragma omp taskwait
  r1 += r2;
}
}

  return r1;
}

// [[Rcpp::export]]
double computeSum_dc_parallel_2(const vec & data, size_t beginI, size_t endI){
  size_t n=endI-beginI;
  if(n==0){
    return 0;
  }
  else if(n==1){
    return data.at(beginI);
  }
  
  size_t half=n/2;
  
  double r1,r2;
  //only one thread exect code below, shared do nothing.
#pragma omp parallel 
{
  //nowait here does nothing.
#pragma omp single
{
#pragma omp task
  r1 = computeSum_dc(data, beginI, beginI + half);
#pragma omp task
  r2 = computeSum_dc(data, beginI + half, endI);
#pragma omp taskwait
  r1 += r2;
}
}

return r1;
}

double parallel_sum(const vec & , size_t ,size_t);
double serial_sum(const vec & , size_t , size_t);

// [[Rcpp::export]]
double sum_divideC(const vec & data){
  double r;
  size_t n=data.n_elem;
  
#pragma omp parallel
#pragma omp single nowait
  r=parallel_sum(data,0,n);
  return r;
}

double parallel_sum(const vec & data, size_t b, size_t n){
  // base case
  if ((n - b) <= CUTOFF) {
    return serial_sum(data, b,  n);
  }
  
  double x,y;
  size_t half=b+(n-b)/2;
#pragma omp task shared(x,data)
  x= parallel_sum(data,b,half);
#pragma omp task shared(y,data)
  y=parallel_sum(data,half,n);
#pragma omp taskwait
  x+=y;
  
  return x;
}

double serial_sum(const vec & data, size_t b,  size_t n)
{
  double s = 0;
  int i;
  for(i = b; i < n; i++)
    s += data(i);
  return s;
}
/*** R
n <- 1e6
data <- runif(n,0,1)
library(microbenchmark)
microbenchmark(computeSum_dc(data,0,n),computeSum_dc_parallel(data,0,n),sum_divideC(data))
computeSum_dc_parallel_2(data,0,n)
*/