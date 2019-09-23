
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

<!-- end list -->

``` r
library(hmmr)
f <- system.file("extdata/volcano1.png", package = "hmmr", mustWork = TRUE)

hmmr:::hmm_triangles(f)
#>   error = 4.65661e-10
#>   points = 160
#>   triangles = 314
#>   vs. naive = 3.04264%
#> list()


hmmr:::hmm_triangles(f, max_triangles = 50)
#>   error = 0.00376332
#>   points = 28
#>   triangles = 50
#>   vs. naive = 0.484496%
#> list()
```

## notes

    ## https://github.com/fogleman/hmm
    fs::dir_copy("../hmm/src", "./src")
    
    tools::package_native_routine_registration_skeleton("../hmmr", "src/init.c",character_only = FALSE)

-----

Please note that the ‘hmmr’ project is released with a [Contributor Code
of
Conduct](https://github.com/hypertidy/hmmr/blob/master/CODE_OF_CONDUCT.md).
By contributing to this project, you agree to abide by its terms.
