// NOTE: 00_hmmr.cpp is the only R package relevant code in hmmr, all the rest is copied from hmm/src/
// MDSumner 2019-09-23

#include <Rcpp.h>
using namespace Rcpp;

#include <functional>
#include <iostream>
#include <string>

#include "base.h"
#include "cmdline.h"
#include "heightmap.h"
#include "stl.h"
#include "triangulator.h"


// hmmr_cpp() is adapted from hmm/src/main.cpp, WIP
// [[Rcpp::export]]
List hmmr_cpp(std::vector< std::string > x,
              LogicalVector invert,
              NumericVector blur_sigma,
              IntegerVector border_size,
              NumericVector border_height,
              NumericVector base_height,
              NumericVector max_error,
              IntegerVector max_triangles,
              IntegerVector max_points,
              NumericVector z_scale,
              NumericVector z_exaggeration,
              LogicalVector quiet
              ) {
  const std::string inFile = (std::string)x[0];
  const auto hm = std::make_shared<Heightmap>(inFile);
  int w = hm->Width();
  int h = hm->Height();
  if (w * h == 0) {
    Rcpp::stop( "invalid heightmap file (try png, jpg, etc.)");
  }

  // invert heightmap
  if (invert[0]) {
    hm->Invert();
  }



  // blur heightmap
  if (blur_sigma[0] > 0) {
    hm->GaussianBlur(blur_sigma[0]);
  }

  // // add border
  if (border_size[0] > 0) {
     hm->AddBorder(border_size[0], border_height[0]);
  }

  // get updated size
  w = hm->Width();
  h = hm->Height();

  // triangulate
 // const float maxError = 0.001;
//  const int maxTriangles = 0;
//  const int maxPoints = 0;
//  const float zScale = 1.0;
//  const float zExaggeration = 1.0;
  Triangulator tri(hm);
  tri.Run(max_error[0], max_triangles[0], max_points[0]);
  auto points = tri.Points(z_scale[0] * z_exaggeration[0]);
  auto triangles = tri.Triangles();

  // // add base
  if (base_height[0] > 0) {
   const float z = -base_height[0] * z_scale[0] * z_exaggeration[0];
    AddBase(points, triangles, w, h, z);
  }

  // display statistics

  if (!quiet[0]) {
    const int naiveTriangleCount = (w - 1) * (h - 1) * 2;
    Rprintf("  error = %g\n", tri.Error());
    Rprintf("  points = %ld\n", points.size());
    Rprintf("  triangles = %ld\n", triangles.size());
    Rprintf("  vs. naive = %g%%\n", 100.f * triangles.size() / naiveTriangleCount);
  }

  // // write output file
  // done = timed("writing output");
  // SaveBinarySTL(outFile, points, triangles);
  // done();

  // // compute normal map
  // if (!normalmapPath.empty()) {
  //   done = timed("computing normal map");
  //   hm->SaveNormalmap(normalmapPath, zScale * zExaggeration);
  //   done();
  // }

  List out = List::create();
  return out;
}
