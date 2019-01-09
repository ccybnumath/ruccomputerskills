#include <Rcpp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>
#include <trng/truncated_normal_dist.hpp>

// [[Rcpp::export]]
double mcIntegration_trancatedNormal(){
  double integral = 0;
  int i;
  int N = 1e5;
  
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  trng::truncated_normal_dist<> u(3,2,0,3);
  for(i = 0; i < N; i++){
    x = u(rx);
    integral += x * x / 3.0 / u.pdf(x);
  }
  
  integral = integral / N;
  return 3*integral;
}
/*** R
library(microbenchmark)
microbenchmark(mcIntegration_trancatedNormal(),mcIntegration_Uniform(),times=50)
*/