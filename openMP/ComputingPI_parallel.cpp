#include <RcppArmadillo.h>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
double computePI(int N){
  double cnt=0;
  int i;
  
#pragma omp parallel  shared(N) private(i)
{
  trng::yarn2 rx;
  double x,y;
  rx.seed(10);
  int size=omp_get_num_threads();
  int rank=omp_get_thread_num();
  trng::uniform_dist<> u(0,1);
#pragma omp for reduction(+:cnt) 
  for(i=0;i<N;i++){
    rx.split(size*2,rank*2);
    x=u(rx);
    rx.split(size*2,rank*2+1);
    y=u(rx);
    if(x*x+y*y<=1)
      cnt+=1;
  }
}
return 4*cnt/N;
}
/*** R
computePI(1e6)
*/