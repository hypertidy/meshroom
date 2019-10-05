test_that("basic usage works", {
  f <- system.file("extdata/volcano1.png", package = "meshroom", mustWork = TRUE)

  expect_silent(expect_equal(meshroom:::hmm_triangles(f, quiet = TRUE), list()))
  expect_output(expect_equal(meshroom:::hmm_triangles(f), list()))

})
