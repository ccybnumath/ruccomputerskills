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
  int i,a = 99;
#pragma omp parallel for private(i) lastprivate(a)
for(i=0;i<10;i++)
  a=i;

printf("i = %d after parallel.\n", i);
printf("a = %d after parallel.\n", a);
return 0;
}
/*** R
main()
*/