test_that("basic usage works", {
  f <- system.file("extdata/volcano1.png", package = "meshr", mustWork = TRUE)

  expect_silent(expect_equal(meshr:::hmm_triangles(f, quiet = TRUE), list()))
  expect_output(expect_equal(meshr:::hmm_triangles(f), list()))

})
