#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

// [[Rcpp::export]]
mat fixedRankApprox(mat A,int K){
  mat U,V,result;
  vec s;
  svd_econ(U,s,V,A);
  
  s.rows(K,s.n_elem-1).zeros();
  result=(U.each_row()%s.t())*V.t();
  result(find(result<0)).zeros();
  result(find(result>1)).ones();
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