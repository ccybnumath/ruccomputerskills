#include<RcppEigen.h>

// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;
using namespace std;

// [[Rcpp::export]]
MatrixXd fixedRankApprox(MatrixXd A,int K){
  
  JacobiSVD<MatrixXd> es(A,ComputeThinU | ComputeThinV);
  VectorXd s=es.singularValues();
  
  //find the k largest singularvalues;
  s.segment(K,s.rows()-K).setZero();

  MatrixXd U=es.matrixU(),V=es.matrixV();
  MatrixXd result(U.rows(),U.cols());
  
  //reconstruct the rank k approximation;
  for(int i=0;i<U.rows();i++){
    result.row(i)=U.row(i).cwiseProduct(s.transpose());
  }
  result*=V.transpose();
  
  //scale to [0,1]
  for(int i=0;i<result.rows();i++)
    for(int j=0;j<result.cols();j++){
      result(i,j)=result(i,j)>1?1:result(i,j)<0?0:result(i,j);
    }
  return result;
}

/*** R
# read in image
library(jpeg)
img = readJPEG("./dog.jpg")
img = img[,,1]

# low rank approximation
img2 = fixedRankApprox(img, 5)  
img3 = fixedRankApprox(img, 20)  
img4 = fixedRankApprox(img, 50)  


## plot two images
par(mar = c(0,0,0,0))
plot(0,0,type = "n")
rasterImage(img, -1, 0, 0, 1)
rasterImage(img2, 0, 0, 1, 1)
rasterImage(img3, -1, -1, 0, 0)
rasterImage(img4, 0, -1, 1, 0)
*/