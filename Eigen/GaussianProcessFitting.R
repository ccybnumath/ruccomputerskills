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
#多边形,首尾相接
polygon(c(sj, rev(sj)), 
        c(fHat[,1]+fHat[,2], rev(fHat[,1]-fHat[,2])),
        col = "lightgrey", border = "white")
lines(sj,x, col = "black", lwd = 2)
lines(sj, fHat[,1], col = "red", lwd = 2)
points(sj[jSet], yVec, col = "black", pch = 20, cex = 0.5)