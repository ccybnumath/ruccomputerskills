# list of matrix kernel
smoothM = matrix(c(1,2,1,2,4,2,1,2,1)/16, 3,3)

library(jpeg)
img = readJPEG("./cat.jpg")
img = img[,,1]
par(mar = c(0,0,0,0)) # plot image
plot(0,0,type = "n") # plot image
rasterImage(img, -1, 0, 1, 1)  # plot image


microbenchmark(imageConv_parallel(img,smoothM),
               imageConv_serial(img, smoothM), times = 10)
