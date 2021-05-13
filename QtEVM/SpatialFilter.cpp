#include "SpatialFilter.h"

bool buildGuassianPyramid(Mat &img, const int levels, std::vector<Mat> &pyrmaid)
{
	
	if (levels<1)
	{
		perror("Levels should be larger than 1");
		return false;
	}
	pyrmaid.clear();
	Mat currrentImg = img;
	for (int l=0;l<levels;l++)
	{
		Mat down;
		pyrDown(currrentImg, down);
		pyrmaid.push_back(down);
		currrentImg = down;
	}
	return false;
}

bool buildLaplacianPyramid(Mat &img, const int levels, std::vector<Mat> &pyrmaid)
{
	if (levels < 1)
	{
		perror("Levels should be larger than 1");
		return false;
	}
	pyrmaid.clear();
	Mat currentImg = img;
	for (int l = 0; l < levels; l++)
	{
		Mat down, up;
		pyrDown(currentImg, down);
		pyrUp(down, up, currentImg.size());
		Mat lap = currentImg - up;
		pyrmaid.push_back(lap);
		currentImg = down;
	}
	pyrmaid.push_back(currentImg);
	return true;
}

void reconmgFromLaplacianPyramid(const std::vector<Mat>& pyramid, const int levels, Mat &dst)
{
	Mat currentImg = pyramid[levels];
	for (int l=levels-1;l>=0;l--)
	{
		Mat up;
		pyrUp(currentImg, up, pyramid[l].size());
		currentImg = up + pyramid[l];
	}
	dst = currentImg.clone();
}

void upsampleFromGuassPyramid(const Mat &src, const int levels, Mat &dst)
{
	Mat currentLevels = src.clone();
	for (int i=0;i<levels;i++)
	{
		Mat up;
		pyrUp(currentLevels, up);
		currentLevels = up;
	}
	currentLevels.copyTo(dst);
}
