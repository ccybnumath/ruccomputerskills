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
 int a=10; 
#pragma omp parallel private(a)
{
  int id = omp_get_thread_num();
  printf("a = %d in thread %d.\n",
         a, id);
  a = 3;
}
return 0;
}
/*** R
main()
*/