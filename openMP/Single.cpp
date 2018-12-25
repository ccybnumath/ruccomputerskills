#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]

void functionA(){
  int id;
  id = omp_get_thread_num();
  printf("Thread %d executes functionA\n",
         id);
}

void functionB(){
  int id;
  id = omp_get_thread_num();
  printf("Thread %d executes functionB\n",
         id);
}


// [[Rcpp::export]]
int main(){
  
#pragma omp parallel 
{
#pragma omp single
  functionA();
  int id = omp_get_thread_num();
  printf("Hi from thread %d.\n",
         id);
}
return 0;
}
/*** R
main()
*/