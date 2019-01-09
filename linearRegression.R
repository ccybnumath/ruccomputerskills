linearRegression=function(y,X){
  nSample=nrow(X)
  pCovariate=ncol(X)
  
  XtX=t(X)%*%X
  XtY=t(X)%*%y
  betaHat = solve(XtX, XtY)
  
  residuals=y-X%*%betaHat
  sigmaHat=sum(residuals*residuals)/(nSample-pCovariate)
  sigmaHat=sqrt(sigmaHat)
  
  betaSigma=sigmaHat*sqrt(diag(solve(XtX)))
  
  result=list(betaHat=betaHat,
              betaSigma=betaSigma,
              sigmaHat=sigmaHat)
  return(result)
}