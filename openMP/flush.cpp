#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo)]]

void read(int *data) {  
  printf("read data\n");  
  *data = 1;  
}  

void process(int *data) {  
  printf("process data\n");  
  (*data)++;  
}

// [[Rcpp::export]]
int main(){
  int data;  
  int flag;  
  
  flag = 0;  
  
  #pragma omp parallel sections num_threads(2)  
  {  
    #pragma omp section  
    {  
        printf("Thread %d: ", omp_get_thread_num( ));  
        read(&data);  
      #pragma omp flush(data)  
        flag = 1;  
      #pragma omp flush(flag)  
        // Do more work.  
    }  
      
    #pragma omp section   
    {  
      while (!flag) {  
      #pragma omp flush(flag)  
      }  
    #pragma omp flush(data)  
      
      printf("Thread %d: ", omp_get_thread_num( ));  
      process(&data);  
      printf("data = %d\n", data);  
    }  
  }
return 0;
}
/*** R
main()
*/