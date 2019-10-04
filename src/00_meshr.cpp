// NOTE: 00_hmmr.cpp is the only R package relevant code in hmmr, all the rest is copied from hmm/src/
// as at https://github.com/fogleman/hmm/tree/49f0a702c7263dd54c63800e60c54a0a79c81959
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

// for getting at triangles
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/normal.hpp>

List hmap(IntegerVector width, IntegerVector height, NumericVector ddd) {
  int len = ddd.length();
  std::vector<float> dat(len);
  for (int i = 0; i < ddd.length(); i++) {
    dat[i] = ddd[i];
  }
  const int w = width[0];
  const int h = height[0];
  const auto hh = std::make_shared<Heightmap>(w, h, dat);

  return List::create();
}
// hmmr_cpp() is adapted from hmm/src/main.cpp, WIP
// [[Rcpp::export]]
List hmmr_cpp(NumericMatrix matr,
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
              LogicalVector quiet,
              std::vector< std::string > stl_file,
              std::vector< std::string > normal_file
              ) {

  int len = matr.length();
  std::vector<float> dat(len);
 // Rprintf("len: %i\n",len);
  for (int i = 0; i < len; i++) {
    dat[i] = matr[i];
  }
  int h = matr.ncol();
  int w = matr.nrow();
//  Rprintf("%i %i\n", w, h);
  const auto hm = std::make_shared<Heightmap>(w, h, dat);


//  const std::string inFile = (std::string)x[0];
//  const auto hm = std::make_shared<Heightmap>(inFile);
//  int w = hm->Width();
//  int h = hm->Height();
  if (w * h < 1) {
    Rcpp::stop( "invalid matrix input");
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

  //double x[3];

  // // write output file
  // done = timed("writing output");
  if (stl_file[0] != "") {
    SaveBinarySTL(stl_file[0], points, triangles);
  }


  // compute normal map
  if (normal_file[0] != "") {

     hm->SaveNormalmap(normal_file[0], z_scale[0] * z_exaggeration[0]);

  }

  List out = List::create();
  return out;
}
