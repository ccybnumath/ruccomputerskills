#include <RcppArmadillo.h>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
void print_random_number_2(){
  int sampleSize=1000;
#pragma omp parallel
{
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  int size=omp_get_num_threads();
  int rank=omp_get_thread_num();
  rx.split((sampleSize/size+1)*rank);
  trng::uniform_dist<> u(0,3);
  x=u(rx);
  printf("Process %d generates %f.\n",  rank, x);
}
}

// [[Rcpp::export]]
void print_random_number(){
#pragma omp parallel
{
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  int size=omp_get_num_threads();
  int rank=omp_get_thread_num();
  rx.split(size,rank);
  trng::uniform_dist<> u(0,3);
  x=u(rx);
  printf("Process %d generates %f.\n",  rank, x);
}
}

// [[Rcpp::export]]
void print_random_number_serial(){
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  trng::uniform_dist<> u(0,3);  // random number distribution
  for(int i = 0; i < 4; i++){
    x = u(rx);
    printf("Generated number: %f.\n", x);
  }
  
}
/*** R
Sys.setenv("PKG_LIBS"="-ltrng4")

*/