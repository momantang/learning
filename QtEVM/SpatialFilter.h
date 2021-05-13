#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "stdio.h"
using namespace cv;

bool buildGuassianPyramid(Mat &img, const int levels, std::vector<Mat> &pyrmaid);
bool buildLaplacianPyramid(Mat &img, const int levels, std::vector<Mat> &pyrmaid);
void reconmgFromLaplacianPyramid(const std::vector<Mat> &pyramid, const int levels, Mat dst);
void upsampleFromGuassPyramid(const std::vector<Mat> &pyramid, const int levels, Mat dst);


