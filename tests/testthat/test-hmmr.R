test_that("basic usage works", {
  f <- system.file("extdata/volcano1.png", package = "hmmr", mustWork = TRUE)

  expect_silent(expect_equal(hmmr:::hmm_triangles(f, quiet = TRUE), list()))
  expect_output(expect_equal(hmmr:::hmm_triangles(f), list()))

})
