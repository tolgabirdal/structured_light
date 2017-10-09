

/*
	This code implements the following paper:
	Grid point extraction and coding for structured light system
	Zhan Song, Ronald Chung, Optical Engineering 50(9), 093602 (September 2011)
*/

#include "Galois.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat genRhombusSLPattern(Galois galois, const int Width = 512, const int Height = 512, const cv::Scalar BGColor = cv::Scalar(255, 255, 255))
{
	if (Width < galois.q || Height < galois.q)
		return cv::Mat();

	cv::Mat Indices, ImagePattern;

	// generate a galois random sequence first
	std::vector<int> GaloisSequence;
	galois.genRandomSequence(GaloisSequence);

	// now fold the sequence into a matrix form
	int foldingWidth = galois.foldingWidth;
	int foldingHeight = galois.fold(GaloisSequence, Indices);

	int N = Width / foldingWidth;
	int M = Height / foldingHeight;
	int foldingLength = foldingWidth*foldingHeight;

	N = std::min(M, N);

	if ((N & 1) == 0) // check even
		N++;

	// now generate the pattern:
	int shiftX = (Width - (foldingWidth - 0.5)*N) / 2;
	int shiftY = (Height - (foldingHeight - 0.5)*N) / 2;

	ImagePattern = cv::Mat(Height, Width, CV_8UC3);
	ImagePattern.setTo(BGColor);
	for (int i = 0; i < foldingHeight; i++) {
		int* indexPtr = Indices.ptr<int>(i);
		int y = (int)(((double)i + 0.5)*N) + shiftY;
		for (int j = 0; j < foldingWidth; j++)
		{
			int x = (int)(((double)j + 0.5)*N) + shiftX;
			// if (x<N || x>Width - N || y<N || y>Height - N)
			// 	continue;

			cv::Scalar color;

			switch (indexPtr[j]) {
			case 0:
				color = cv::Scalar(255, 255, 255)-BGColor;
				break;
			case 1:
				color = cv::Scalar(0, 0, 255);
				break;
			case 2:
				color = cv::Scalar(0, 255, 0);
				break;
			case 3:
				color = cv::Scalar(255, 0, 0);
				break;
			default:
				color = cv::Scalar(255, 255, 255);
				break;
			};

			cv::Point diamondPos(x, y);
			cv::Point pts[4] = { cv::Point(x, y - N / 2), cv::Point(x + N / 2, y),
								 cv::Point(x, y + N / 2), cv::Point(x - N / 2, y) };
			cv::fillConvexPoly(ImagePattern, pts, 4, color);
		}
	}
	return ImagePattern;
}

int main()
{
	Galois GaloisSeqGenerator;
	cv::Mat Pattern = genRhombusSLPattern(GaloisSeqGenerator, 800, 600);

	cv::imshow("Random Galois Pattern", Pattern);
	cv::waitKey();

	return 0;
}