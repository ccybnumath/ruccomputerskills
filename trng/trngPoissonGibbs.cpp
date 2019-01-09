#include <RcppArmadillo.h>
#include <RcppArmadilloExtensions/sample.h>
#include <trng/yarn2.hpp>
#include <trng/gamma_dist.hpp>
// [[Rcpp::depends(RcppArmadillo)]]

using namespace arma;

double factor(int n){
  if(n==1)
    return 1;
  return n*factor(n-1);
}

vec poissonDensity(vec x,double lambda){
  vec temp(x.n_elem,fill::zeros);
  for(int k=0;k<x.n_elem;k++){
    temp(k)=R::dpois(x(k),lambda,FALSE);
  }
  return temp;
}

// [[Rcpp::export]]
mat poissonGibbs(double a, double b, int N, vec x, int burnIn,int mcmcN){
  int i;
  mat samples(3,mcmcN);
  vec probn(N,fill::zeros);
  vec Theta(3,fill::zeros);//lambda1 Theta(0),lambda2 Theta(1), n Theta(2) range from 1 to N;
  Theta(2)=N/2;
  int n=N/2;
  
  trng::yarn2 rx;
  rx.seed(10);
  //init
  //shape,rate
  
  trng::gamma_dist<> p1(sum(x(span(0,n)))+a,1/(n+b)),p2(sum(x(span(n+1,N-1)))+a,1/(N-n+b));
  
  for(i=0;i<burnIn+ mcmcN;i++){
    
    //update Theta(0)
    p1.kappa(sum(x(span(0,Theta(2)-1)))+a);
    p1.theta(1/(Theta(2)+b));
    Theta(0)=p1(rx);
    
    //update Theta(1)
    if(Theta(2)<N){
      p2.kappa(sum(x(span(Theta(2),N-1)))+a);
      p2.theta(1/(N-Theta(2)+b));
    }
    else{
      p2.kappa(a);
      p2.theta(b);
    }
    Theta(1)=p2(rx);
    
    //update Theta(2);
    //compute mass density
    for(int j=0;j<N-1;j++){
      probn(j)=prod(poissonDensity(x(span(0,j)),Theta(0)))*prod(poissonDensity(x(span(j+1,N-1)),Theta(1)));
    }
    probn(N-1)=prod(poissonDensity(x(span(0,N-1)),Theta(0)));
    
    probn/=sum(probn);
    vec fullvec = regspace<vec>(1,N);
    Theta(2)=sum(Rcpp::RcppArmadillo::sample(fullvec,1,true,probn));
    
    
    //record
    if(i>=burnIn) samples.col(i-burnIn)=Theta;
    
  }
  return samples;
  
}
/*** R
lambda_1 = 10
lambda_2 = 18
n = 60
N = 100

x1 = rpois(n, lambda_1)
x2 = rpois(N-n, lambda_2)
dataX = c(x1, x2)
plot(dataX, xlab = "", ylab = "", pch = 20)

samples <- poissonGibbs(1.5,0.1,N,c(x1,x2),10000,10000)
samples = t(samples)
samples = data.frame(samples)
colnames(samples) = c("lambda1", "lambda2","N")
hist(samples$lambda1)
hist(samples$lambda2)
hist(samples$N)
*/