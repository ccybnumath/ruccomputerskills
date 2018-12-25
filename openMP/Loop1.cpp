#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]


// [[Rcpp::export]]
int main(){
  int i,n;
  n=40;
#pragma omp parallel for shared(n) private(i)
for(i=0;i<n;i++){
    int id;
    id = omp_get_thread_num();
    printf("Thread %d executes loop iteration %d\n",
           id, i);
  
}
printf("Parallel ends.\n");

}
/*** R
main()
*/