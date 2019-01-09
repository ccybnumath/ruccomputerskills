#include <RcppEigen.h>
// [[Rcpp::depends(RcppEigen)]]
using namespace Eigen;
using namespace std;

// [[Rcpp::export]]
void  BasicOp(MatrixXd A){
  cout<<"Determinant: "<<A.determinant()<<endl;
  cout<<"Inverse: "<<A.inverse()<<endl;
  cout<<"Transpose: "<<A.transpose()<<endl;
  cout<<"Conjugate: "<<A.conjugate()<<endl;
  cout<<"Adjoint: "<<A.adjoint()<<endl;
}
/*** R
A <- matrix(c(1,0,0,2),nrow=2)
BasicOp(A)
*/