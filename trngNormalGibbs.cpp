#include <RcppArmadillo.h>
#include <trng/yarn2.hpp>
#include <trng/normal_dist.hpp>
// [[Rcpp::depends(RcppArmadillo)]]

using namespace arma;

// [[Rcpp::export]]
mat normalGibbs(vec mu, mat Sigma, int burnIn,int mcmcN){
  int i;
  mat samples(2,mcmcN);
  vec theta(2,fill::zeros);
  double mu12,sigma12,mu21,sigma21;
  sigma12 = Sigma(0,0) - Sigma(0,1) * Sigma(0,1) / Sigma(1,1);
  sigma12 = sqrt(sigma12);
  sigma21 = Sigma(1,1) - Sigma(0,1) * Sigma(0,1) / Sigma(0,0);
  sigma21 = sqrt(sigma21);
  
  trng::yarn2 rx;
  rx.seed(10);
  //init
  trng::normal_dist<> u1(0.0,sigma12),u2(0.0,sigma21);
  
  for(i=0;i<burnIn+ mcmcN;i++){
    //compute mean value
    mu12 = mu(0) + Sigma(0,1) / Sigma(1,1) * (theta(1) - mu(1));
    //set new mean value
    u1.mu(mu12);
    //get new sample
    theta(0)=u1(rx);
    
    // compute mean value
    mu21 = mu(1) + Sigma(0,1) / Sigma(0,0) * (theta(0) - mu(0));
    // set new mean value
    u2.mu(mu21);
    // get new sample
    theta(1) = u2(rx);
    
    // record samples after the burnin period
    if(i>=burnIn) samples.col(i-burnIn)=theta;
    
    
  }
  return samples;
}