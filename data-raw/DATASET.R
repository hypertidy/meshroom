## code to prepare `DATASET` dataset goes here

d <- raster::setExtent(raster::raster(volcano), raster::extent(0, ncol(volcano), 0, nrow(volcano)))
d <- (d - raster::cellStats(d, min))/(raster::cellStats(d, max) - raster::cellStats(d, min))
rgdal::writeGDAL(as(d, "SpatialGridDataFrame"), "inst/extdata/volcano1.png", drivername = "PNG")
