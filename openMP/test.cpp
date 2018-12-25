#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]


// [[Rcpp::export]]
int main(){
  int total=omp_get_max_threads();
  #pragma omp parallel
  {
    //parallel region construct
    int id = omp_get_thread_num();
    printf("Greetings from process %d out of %d.\n",id,total);
    //cout << "Greetings from process " << id << " out of " << total <<endl;//cause chaos.
  }
  printf("Parallel ends.\n");

}
/*** R
main()
*/