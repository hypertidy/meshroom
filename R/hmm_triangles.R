#' Heightmap meshing
#'
#' Create an efficient triangulation from a height map.
#'
#' Currently input must be a matrix.
#'
#' @param x matrix
#' @param invert  logical, invert heightmap, default is `FALSE`
#' @param blur_sigma gaussian blur sigma, default is `0` for no blur
#' @param border_size border size in pixels, default is `0`
#' @param border_height border z height, default is `1`
#' @param base_height solid base height, default is no base `0`
#' @param max_error maximum triangulation error, default is `0.001`
#' @param max_triangles maximum number of triangles, default is no maximum `0`
#' @param max_points maximum number of vertices, default is no maximum `0`
#' @param z_scale z scale relative to x, y
#' @param z_exaggeration z exaggeration, default is `1`
#' @param quiet report metrics from triangulation, default is `TRUE`
#' @param stl_file if set the triangulation will be written to file, default is no write `""`
#' @param auto_level if `TRUE` input matrix is rescaled to full grayscale range
#' @param normal_file if set the normal map will be written to file, default is no write `""`
#'
#' @return matrix of triangle vertices
#' @export
#'
#' @examples
#' f <- system.file("extdata/volcano1.png", package = "meshroom", mustWork = TRUE)
#' im <- 255 * (volcano - min(volcano))/diff(range(volcano))
#' #tfile <- tempfile()
#' ## tris and o below should be the same
#' o <- meshroom:::hmm_triangles(im, stl_file = tfile)
#' tris <- rgl::readSTL(tfile, plot = FALSE)
hmm_triangles <- function(x,  invert = FALSE,
                          blur_sigma = 0,
                          border_size = 0,
                          border_height = 1,
                          base_height = 0,
                          auto_level = TRUE,
                          max_error = 0.001,
                          max_triangles = 0,
                          max_points = 0,
                          z_scale = 1,
                          z_exaggeration = 1,
                          quiet = FALSE,
                          stl_file = "",
                          normal_file = "") {
  if (nchar(stl_file) > 0) {
    if (file.exists(stl_file)) {
      stop(sprintf("file '%s' already exists, please delete or specify a different file for output", stl_file))
    }
  }
  if (nchar(normal_file) > 0) {
    if (file.exists(normal_file)) {
      stop(sprintf("file '%s' already exists, please delete or specify a different file for output", normal_file))
    }
  }
  result <- hmmr_cpp(x,
           invert = invert,
           blur_sigma = blur_sigma,
           border_size = border_size,
           border_height = border_height,
           base_height = base_height,
           auto_level = auto_level,
           max_error = max_error,
           max_triangles = max_triangles,
           max_points = max_points,
           z_scale = z_scale,
           z_exaggeration = z_exaggeration,
           quiet = quiet,
           stl_file = stl_file,
           normal_file = normal_file
           )
  ## unpack raw bytes and return matrix
  nn <- length(result[[1]])
  invisible(matrix(readBin(result[[1]], what = "numeric", n = nn/4, size = 4),
         ncol = 3L, byrow = TRUE))
}
