#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]


// [[Rcpp::export]]
int main(){
  double a;
#pragma omp parallel shared(a)
{
  int id = omp_get_thread_num();
  a=id;
  sleep(1);
  printf("Thread %d has a =  %f\n",
         id, a);
}
return 0;
}
/*** R

*/