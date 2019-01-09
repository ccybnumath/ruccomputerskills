#include <Rcpp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>
#include <trng/truncated_normal_dist.hpp>


// [[Rcpp::export]]
double mcIntegration_Uniform(){
  double integral=0;
  int i;
  int N=1e7;
  
  trng::yarn2 rx;
  double x;
  rx.seed(10);
  trng::uniform_dist<> u(0,3);
  
  for(i=0;i<N;i++){
    x=u(rx);
    integral+=x*x;
  }
  return 3*integral/N;
}