
<!-- README.md is generated from README.Rmd. Please edit that file -->

# hmmr

<!-- badges: start -->

[![Travis build
status](https://travis-ci.org/hypertidy/hmmr.svg?branch=master)](https://travis-ci.org/hypertidy/hmmr)
<!-- badges: end -->

The goal of hmmr is to provide the heightmap meshing facility of the
[hmm library](https://github.com/fogleman/hmm) to R.

It currently does not do anything *unless `file_stl` is specified*, the
meshing is run but nothing is returned but an empty list and a message.

TODO

  - DONE: STL output
  - allow input from in memory matrix, and maintain real world z values
  - capture output triangles
  - sort out use of hmm, see notes below and in 00\_hmmr.cpp
  - DONE (see below): maintain mapping for geospatial coordinates

## Installation

Only on Linux.

    sudo apt-get install libglm-dev

``` r
remotes::install_github("hypertidy/hmmr")
```

## Example

``` r
library(hmmr)
f <- system.file("extdata/volcano1.png", package = "hmmr", mustWork = TRUE)

hmmr::hmm_triangles(f)
#>   error = 5.96046e-08
#>   points = 3466
#>   triangles = 6769
#>   vs. naive = 65.5911%
#> list()


hmmr::hmm_triangles(f, max_triangles = 50)
#>   error = 0.0638039
#>   points = 29
#>   triangles = 51
#>   vs. naive = 0.494186%
#> list()
```

Now write to STL so we can [check it
out](https://github.com/hypertidy/hmmr/blob/master/man/figures/volcano1.stl).

``` r
hmmr::hmm_triangles(f, z_exaggeration = 30, stl_file = "man/figures/volcano1.stl")
```

## notes

``` 
## https://github.com/fogleman/hmm
fs::dir_copy("../hmm/src", "./src")

tools::package_native_routine_registration_skeleton("../hmmr", "src/init.c",character_only = FALSE)

```

``` r
## local testing
library(raster)
d <- raadtools::readtopo("etopo2", xylim = raster::extent(100, 180, -70, -30))
d <- d - cellStats(d, min)
d <- d / (cellStats(d, max)/256)
d <- aggregate(d, fact = 8)
rgdal::writeGDAL(as(d, "SpatialGridDataFrame"), "etopo.png", drivername = "PNG")
unlink("stl.stl"); hmmr:::hmm_triangles("etopo.png",  stl_file = "stl.stl")
rgl::rgl.clear(); r <- rgl::readSTL("stl.stl", plot = TRUE, col = "grey", lit = TRUE); rgl::aspect3d(1, 1, .2); rgl::rglwidget()
```

![alt text](man/figures/topo.png
"Etopo2 / 8 as 77053 triangles in 38861 points")

## Example of restoring geospatial information.

``` r
library(ceramic)
ex <- raster::extent(144, 149, -44, -40)
el <- cc_elevation(ex, zoom = 5)
el[el < 1] <- NA

el <- el - cellStats(el, min)
el <- el / (cellStats(el, max)/256)

tfile <- tempfile(fileext = ".png")
rgdal::writeGDAL(as(el, "SpatialGridDataFrame"), tfile, drivername = "PNG")
sfile <- tempfile(fileext = ".stl")
hmmr:::hmm_triangles(tfile,  stl_file = sfile, z_scale = TRUE)
tris <- rgl::readSTL(sfile, plot = FALSE)

## now re-map geographic coordinates back int
library(raster)
cell <- cellFromXY(setExtent(el, raster::extent(1, ncol(el), 1, nrow(el))), 
                   tris[,1:2] + 1)


tris[,1:2] <- xyFromCell(el, cell)

## and unproject just for fun
tris[,1:2] <- rgdal::project(tris[,1:2], projection(el), inv = TRUE)
rgl::rgl.clear(); rgl::triangles3d(tris[,1], tris[,2], tris[,3], 
                                   col = rep(c("white", "grey", "black", "darkgrey"), each = 3));
rgl::aspect3d(1, 1, .035); rgl::rglwidget()
rgl::par3d()$bbox
```

![alt text](man/figures/geo_triangles.png
"Tasmania triangulated in longlat")

-----

Please note that the ‘hmmr’ project is released with a [Contributor Code
of
Conduct](https://github.com/hypertidy/hmmr/blob/master/CODE_OF_CONDUCT.md).
By contributing to this project, you agree to abide by its terms.
