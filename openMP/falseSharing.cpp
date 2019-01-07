#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]


// [[Rcpp::export]]
int find_parallel(vec data){
  uvec sumVec(16,fill::zeros);
  omp_set_num_threads(4);
#pragma omp parallel shared(data,sumVec)
{
  int p=omp_get_thread_num();
#pragma omp for schedule(static)
  for(uword i=0;i<data.n_elem;++i){
    if(data(i)>0.5)
      ++sumVec(p);
  }
}
  return sum(sumVec);
}

// [[Rcpp::export]]
int find_parallel_2(vec data){
  uvec sumVec(16, fill::zeros);
  omp_set_num_threads(4);
#pragma omp parallel shared(data,sumVec)
{
  int localSum = 0;
  int p = omp_get_thread_num();
  printf_s("%d\n",p);
#pragma omp for schedule(static)
  for( uword i = 0; i < data.n_elem; ++i ){
    if( data(i) > 0.5 )
      ++localSum;
    printf_s("  :%d\n",p);
  }
    
    sumVec(p) = localSum;
}

return sum(sumVec);
}

// [[Rcpp::export]]
int find_serial(vec data){
  int odds = 0;
  for( uword i = 0; i < data.n_elem; ++i )
    if( data(i) > 0.5 )
      ++odds;
    return odds;
}

/*** R
library(microbenchmark)
data <- runif(10,0,1)
find_parallel_2(data)
#microbenchmark(find_parallel(data),find_parallel_2(data),find_serial(data))
*/