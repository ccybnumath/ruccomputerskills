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
  int i,id,n = 99;
#pragma omp parallel for shared(n) private(i,id) schedule(dynamic,2)
  for(i=0;i<n;i++){
    id = omp_get_thread_num();
    printf("Thread %d executes loop iteration %d\n",
           id, i);
  }
    
  return 0;
}
/*** R
main()
*/