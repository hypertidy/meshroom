
<!-- README.md is generated from README.Rmd. Please edit that file -->

# hmmr

<!-- badges: start -->

<!-- badges: end -->

The goal of hmmr is to provide the heightmap meshing facility of the
[hmm library](https://github.com/fogleman/hmm) to R.

It currently does not do anything, the meshing is run but nothing is
returned but an empty list and a message.

TODO

  - allow input from in memory matrix
  - capture output triangles
  - sort out use of hmm, see notes

<!-- end list -->

``` r
library(hmmr)
f <- system.file("extdata/volcano1.png", package = "hmmr", mustWork = TRUE)

hmmr:::hmm_triangles(f)
#>   error = 5.96046e-08
#>   points = 3466
#>   triangles = 6769
#>   vs. naive = 65.5911%
#> list()


hmmr:::hmm_triangles(f, max_triangles = 50)
#>   error = 0.0638039
#>   points = 29
#>   triangles = 51
#>   vs. naive = 0.494186%
#> list()
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

-----

Please note that the ‘hmmr’ project is released with a [Contributor Code
of
Conduct](https://github.com/hypertidy/hmmr/blob/master/CODE_OF_CONDUCT.md).
By contributing to this project, you agree to abide by its terms.
