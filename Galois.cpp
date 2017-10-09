
#include "Galois.hpp"

void Galois::genRandomSequence(std::vector<int>& GaloisSequence)
{
	const int l = (int)pow((double)q, (double)degree) - 1;
	std::vector<int> seed(degree, 0);
	int* primPoly1, i, j;

	seed[0] = 1; // fill the first one with 1
	primPoly1 = &PrimPoly[1];

	GaloisSequence.resize(l);

	// generate sequence
	for (i = 0; i<l; i++) {
		int data = 0, bit;
		for (j = 0; j<degree; j++) {
			const int xj = ProductTable[seed[j] * 4 + primPoly1[j]];
			data = AdditionTable[data * 4 + xj];
		}

		bit = seed[degree - 1];

		// shift
		for (j = degree - 2; j >= 0; j--)
			seed[j + 1] = seed[j];

		seed[0] = data;
		GaloisSequence[i] = bit;
	}
}

int Galois::fold(std::vector<int> Sequence, cv::Mat& FoldedMatrix)
{
	const int foldingHeight = Sequence.size() / foldingWidth;
	int x = 0, y = 0, i;

	FoldedMatrix = cv::Mat(foldingHeight, foldingWidth, CV_32S);
	for (i = 0; i<Sequence.size(); i++)
	{
		x = x % foldingWidth;
		y = y % foldingHeight;
		FoldedMatrix.at<int>(y, x) = Sequence[i];
		x++; y++;
	}

	return foldingHeight;
}