#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]


// [[Rcpp::export]]
void race_task(){
#pragma omp parallel
{
#pragma omp single
{
  printf_s("A ");
#pragma omp task
  printf_s("race ");
#pragma omp task
  printf_s("car ");
  printf_s("is fun to watch.");
}
  
}
}

// [[Rcpp::export]]
void race_task_wait(){
#pragma omp parallel
{
#pragma omp single
{
  printf_s("A ");
#pragma omp task
  printf_s("race ");
#pragma omp task
  printf_s("car ");
#pragma omp taskwait
  printf_s("is fun to watch.");
}
  
}
}


/*** R
for(i in 1:100){
  race_task()
  cat("\n")
}

for(i in 1:100){
  race_task_wait()
  cat("\n")
}
*/