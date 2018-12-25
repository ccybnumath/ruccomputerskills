#include <RcppEigen.h>
#include <vector>
using namespace Eigen;
using namespace std;
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(RcppEigen)]]

typedef Eigen::Triplet<double> T;
typedef SparseMatrix<double> SpMat;

SpMat createLmat(int D){
  SpMat Lmat(D-2,D);
  // VectorXd::Constant(D, 3) is a vector of size D, and = (3,3,...,3)^T
  //Preallocates reserveSize[j] non zeros for each column (resp. row) j.
  Lmat.reserve(VectorXd::Constant(D,3));
  int i,j,k;
  for(j=0;j<D;j++){
    for(i=-1;i<2;i++){
      k=i+j-1;
      if(k<0||k>D-3) continue;
      if(i==0)
        Lmat.insert(k,j)=1;
      else
        Lmat.insert(k,j)=-0.5;
    }
  }
  return Lmat;
}

// Create the observation matrix A.
SpMat createAmat(VectorXd jSet,int D){
  SpMat Amat(jSet.size(),D);
  int j;
  vector<T> tripletList;
  tripletList.reserve(jSet.size());
  for(j=0;j<jSet.size();j++){
    tripletList.push_back(T(j,jSet(j),1));
  }
  
  Amat.setFromTriplets(tripletList.begin(),tripletList.end());
  return Amat;
  
}

// [[Rcpp::export]]
MatrixXd gpFitting(VectorXd jSet, VectorXd yVec, 
                   double lambdaSq, double sigmaSq_y, int D) {
  // The conditional mean, and its standard deviation uncertainty
  VectorXd mu,muSigma;
  
  SpMat Lmat,Amat,SigmaXYInverse;
  Lmat=createLmat(D);
  Amat=createAmat(jSet,D);
  
  SigmaXYInverse=lambdaSq*Lmat.transpose()*Lmat+Amat.transpose()*Amat/sigmaSq_y;
  
  // sparse solve to get the conditional mean
  Eigen::SimplicialLLT<SpMat> solver(SigmaXYInverse);
  // take inverse to get uncertainty.
  mu=Amat.transpose()*yVec/sigmaSq_y;
  mu=solver.solve(mu);
  
  // compute the conditional variance
  MatrixXd Imat(D, D);
  Imat.setIdentity();
  auto SigmaSparse = solver.solve(Imat); // to get the inverse
  //MatrixXd SigmaDense = MatrixXd(SigmaSparse);
  //SigmaSparse.di
  muSigma = SigmaSparse.diagonal();
  
  MatrixXd result(D, 2);
  result << mu , muSigma.cwiseSqrt();
  return result;

}
/*** R
D = 5000
n = 100
sigmaSq_y = 0.1

sj = seq(0, 1, length.out = D)
x = sin(6*pi*sj)

jSet = sample(1:D, n, replace = FALSE)
yVec = x[jSet] + rnorm(n, sd = sqrt(sigmaSq_y))

#microbenchmark::microbenchmark(gpFitting(jSet - 1, yVec, 1e7, sigmaSq_y, D), times = 10)
fHat = gpFitting(jSet - 1, yVec, 1e8, sigmaSq_y, D)

plot(sj[jSet], yVec, pch = 20, cex = 0.5, type = "n")
//多边形,首尾相接
polygon(c(sj, rev(sj)), 
        c(fHat[,1]+fHat[,2], rev(fHat[,1]-fHat[,2])),
        col = "lightgrey", border = "white")
lines(sj,x, col = "black", lwd = 2)
lines(sj, fHat[,1], col = "red", lwd = 2)
points(sj[jSet], yVec, col = "black", pch = 20, cex = 0.5)
*/
