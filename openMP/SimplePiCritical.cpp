#include <RcppArmadillo.h>
#include <omp.h>

using namespace arma;
using namespace std;
// [[Rcpp::plugins(openmp)]]
// [[Rcpp::depends(RcppArmadillo, RcppDist)]]


#define NUM_THREADS 2
#define PAD 16// assume 128 byte L1 cache line size 

// [[Rcpp::export]]
double main1()
{ static long num_steps=100000000;
  double step;
  step=1.0/(double) num_steps;
  int i,nthreads;
  double pi,sum[NUM_THREADS];
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
  int i,id,nthrds;
  double x;
  id=omp_get_thread_num();
  nthrds=omp_get_num_threads();
  if(id==0) nthreads=nthrds;
  for(i=id,sum[id]=0.0;i<num_steps;i=i+nthrds){
    x=(i+0.5)*step;
    sum[id]+=4.0/(1.0+x*x);
  }
}
for(i=0,pi=0.0;i<nthreads;i++) pi+=sum[i]*step;
//printf("%lf",pi);
return pi;
}


// [[Rcpp::export]]
double main2()
{ static long num_steps=1000000;
  double step;
  step=1.0/(double) num_steps;
  int i,nthreads;
  double pi,sum[NUM_THREADS][PAD];
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
  int i,id,nthrds;
  double x;
  id=omp_get_thread_num();
  nthrds=omp_get_num_threads();
  if(id==0) nthreads=nthrds;
  for(i=id,sum[id][0]=0.0;i<num_steps;i=i+nthrds){
    x=(i+0.5)*step;
    sum[id][0]+=4.0/(1.0+x*x);
  }
}
for(i=0,pi=0.0;i<nthreads;i++) pi+=sum[i][0]*step;
//printf("%lf",pi);
return pi;
}


// [[Rcpp::export]]
double main3()
{ static long num_steps=1000000;
  double step;
  step=1.0/(double) num_steps;
  int i,nthreads;
  double pi,sum;
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
{
  int i,id,nthrds;
  double x;
  id=omp_get_thread_num();
  nthrds=omp_get_num_threads();
  if(id==0) nthreads=nthrds;
  for(i=id;i<num_steps;i=i+nthrds){
    x=(i+0.5)*step;
    sum+=4.0/(1.0+x*x);
  }
#pragma omp critical
  pi+=sum*step;
}
//printf("%lf",pi);
return pi;
}

/*** R
library(microbenchmark)
microbenchmark(main1(),main2(),main3())
*/