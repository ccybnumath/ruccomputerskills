#include <RcppArmadillo.h>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
double mcIntegration_parallel_Uniform(){
  double integral=0;
  int N=1e6;
  int i;
  omp_set_num_threads(16);
  
#pragma omp parallel  shared(N) private(i)
{
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  int size=omp_get_num_threads();
  int rank=omp_get_thread_num();
  rx.split(size,rank);
  trng::uniform_dist<> u(0,3);
#pragma omp for reduction(+:integral) 
  for(i=0;i<N;i++){
    x=u(rx);
    integral+=x*x;
  }
}
  integral = 3.0 * integral / N;
  return integral;
}

/*** R
Sys.setenv("PKG_LIBS"="-ltrng4")
mcIntegration_parallel_Uniform()
*/