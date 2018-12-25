library(microbenchmark)
set.seed(100)
n = 1e7
x = 2*pi*(1:n)/n
y = sin(x) + rnorm(n, sd = 0.1)

weights = c(1,2,3,4,5,4,3,2,1) /25

microbenchmark(localSmoothing_serial(y, weights) , 
               localSmoothing_parallel(y, weights),
               times = 10, unit = "ms")
