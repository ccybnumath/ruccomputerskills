#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace arma;

inline int mirrorIndex(int fetchI, int length){
  if(fetchI < 0)
    fetchI = -fetchI - 1;
  if(fetchI >= length){
    fetchI = length - (fetchI - length) - 1;
  }
  return fetchI;
}


// [[Rcpp::export]]
mat imageConv_serial(mat img,  mat kernel){
  int m, n;
  m = img.n_rows;
  n = img.n_cols;
  mat img_result(m,n);
  
  int i, j, h, k, imgI, imgJ;
  double fin;
  for(j = 0; j < n; j++)
    for(i = 0; i < m; i++){
      fin = 0;
      for(h = -1; h < 2; h++)
        for(k = -1; k < 2; k++){
          imgI = i + h;
          imgJ = j + k;
          imgI = mirrorIndex(imgI, m);
          imgJ = mirrorIndex(imgJ, n);
          fin += img(imgI,imgJ) * kernel(h+1, k+1);
        }
        img_result(i,j) = fin;
    }
    
    img_result(find(img_result > 1)).ones();
  img_result(find(img_result < 0)).zeros();
  
  return img_result;
}

// [[Rcpp::export]]
mat imageConv_parallel(mat img,  mat kernel){
  int m, n;
  m = img.n_rows;
  n = img.n_cols;
  mat img_result(m,n);
  
  int i, j, h, k, imgI, imgJ;
  double fin;
#pragma omp parallel for schedule(static) private(i,j,fin,h,k,imgI,imgJ) \
  shared(m,n,img_result,img,kernel) \
    default(none)
  for(j = 0; j < n; j++)
    for(i = 0; i < m; i++){
      fin = 0;
      for(h = -1; h < 2; h++)
        for(k = -1; k < 2; k++){
          imgI = i + h;
          imgJ = j + k;
          imgI = mirrorIndex(imgI, m);
          imgJ = mirrorIndex(imgJ, n);
          fin += img(imgI,imgJ) * kernel(h+1, k+1);
        }
        img_result(i,j) = fin;
    }
    
    img_result(find(img_result > 1)).ones();
  img_result(find(img_result < 0)).zeros();
  
  return img_result;
}

// [[Rcpp::export]]
mat imageConv_unrolling(mat img,  mat kernel){
  int m, n;
  m = img.n_rows;
  n = img.n_cols;
  mat img_result(m,n);
  
  int i, j, h, k, imgI, imgJ;
  double fin;
  for(j = 0; j < n; j++)
    for(i = 0; i < m; i++){
      fin = 0;
      imgI = mirrorIndex(i - 1, m);
      imgJ = mirrorIndex(j - 1, n);
      fin += img(imgI,imgJ) * kernel(0, 0);
      
      imgI = mirrorIndex(i - 1, m);
      imgJ = mirrorIndex(j, n);
      fin += img(imgI,imgJ) * kernel(0, 1);
      
      imgI = mirrorIndex(i - 1, m);
      imgJ = mirrorIndex(j + 1, n);
      fin += img(imgI,imgJ) * kernel(0, 2);
      
      imgI = mirrorIndex(i, m);
      imgJ = mirrorIndex(j -1 , n);
      fin += img(imgI,imgJ) * kernel(1, 0);
      
      imgI = mirrorIndex(i, m);
      imgJ = mirrorIndex(j , n);
      fin += img(imgI,imgJ) * kernel(1, 1);
      
      
      imgI = mirrorIndex(i, m);
      imgJ = mirrorIndex(j + 1, n);
      fin += img(imgI,imgJ) * kernel(1, 2);
      
      
      imgI = mirrorIndex(i + 1, m);
      imgJ = mirrorIndex(j - 1, n);
      fin += img(imgI,imgJ) * kernel(2, 0);
      
      imgI = mirrorIndex(i + 1, m);
      imgJ = mirrorIndex(j , n);
      fin += img(imgI,imgJ) * kernel(2, 1);
      
      imgI = mirrorIndex(i + 1, m);
      imgJ = mirrorIndex(j + 1, n);
      fin += img(imgI,imgJ) * kernel(2, 2);
      
      img_result(i,j) = fin;
    }
    
    img_result(find(img_result > 1)).ones();
  img_result(find(img_result < 0)).zeros();
  
  return img_result;
}

// [[Rcpp::export]]
mat imageConv_parallel2(mat img,  mat kernel){
  int m, n;
  m = img.n_rows;
  n = img.n_cols;
  mat img_result(m,n);
  
  int i, j, h, k, imgI, imgJ;
  double fin;
#pragma omp parallel for schedule(static)     \
  private(i,j,h,k,imgI,imgJ,fin)              \
    shared(m,n,img_result, img, kernel)       \
    default(none) 
    for(j = 0; j < n; j++)
#pragma omp simd
      for(i = 0; i < m; i++){
        fin = 0;
        fin = 0;
        imgI = mirrorIndex(i - 1, m);
        imgJ = mirrorIndex(j - 1, n);
        fin += img(imgI,imgJ) * kernel(0, 0);
        
        imgI = mirrorIndex(i - 1, m);
        imgJ = mirrorIndex(j, n);
        fin += img(imgI,imgJ) * kernel(0, 1);
        
        imgI = mirrorIndex(i - 1, m);
        imgJ = mirrorIndex(j + 1, n);
        fin += img(imgI,imgJ) * kernel(0, 2);
        
        imgI = mirrorIndex(i, m);
        imgJ = mirrorIndex(j -1 , n);
        fin += img(imgI,imgJ) * kernel(1, 0);
        
        imgI = mirrorIndex(i, m);
        imgJ = mirrorIndex(j , n);
        fin += img(imgI,imgJ) * kernel(1, 1);
        
        
        imgI = mirrorIndex(i, m);
        imgJ = mirrorIndex(j + 1, n);
        fin += img(imgI,imgJ) * kernel(1, 2);
        
        
        imgI = mirrorIndex(i + 1, m);
        imgJ = mirrorIndex(j - 1, n);
        fin += img(imgI,imgJ) * kernel(2, 0);
        
        imgI = mirrorIndex(i + 1, m);
        imgJ = mirrorIndex(j , n);
        fin += img(imgI,imgJ) * kernel(2, 1);
        
        imgI = mirrorIndex(i + 1, m);
        imgJ = mirrorIndex(j + 1, n);
        fin += img(imgI,imgJ) * kernel(2, 2);
        
        img_result(i,j) = fin;
      }
      
      
      img_result(find(img_result > 1)).ones();
  img_result(find(img_result < 0)).zeros();
  return img_result;
}

