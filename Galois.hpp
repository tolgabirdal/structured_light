
/*
	This file implements the beautiful Galois algebra for sequence generation.
	For more information consult: 
	F. J. MacWilliams and N. J. A. Sloane, "Pseudorandom sequences and arrays" 
	Proc. IEEE 64(12), 1715–1729 (1976).
	by Tolga Birdal
*/

#pragma once

#ifndef _T_GALOIS_FIELD_HPP_
#define _T_GALOIS_FIELD_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

static int AdditionTable[16] = { 0,1,2,3,1,0,3,2,2,3,0,1,3,2,1,0 };
static int ProductTable[16] = { 0,0,0,0,0,1,2,3,0,2,3,1,0,3,1,2 };

// implemented in C level for speed and general use
static void galois_add(const std::vector<int>& A, const std::vector<int>& B, const int N, const int Q, std::vector<int>& APlusB)
{
	for (int i = 0; i<N; i++)
		APlusB[i] = AdditionTable[(A[i] << 2) + B[i]];
}

static void galois_mul(const std::vector<int>& A, const std::vector<int>& B, const int N, const int Q, std::vector<int>& ATimesB)
{
	for (int i = 0; i<N; i++)
		ATimesB[i] = ProductTable[(A[i] << 2) + B[i]];
}

static int galois_sum(const std::vector<int>& A, const int N, const int Q, std::vector<int>& APlusB)
{
	int sum = 0;
	for (int i = 0; i<N; i++)
		sum += AdditionTable[(sum << 2) + A[i]];
}

// only supports degree 6!
class Galois
{
public:
	Galois() : q(4), foldingWidth(63)
	{
		degree = 6;
		PrimPoly = std::vector<int>(degree + 1, 0);
		PrimPoly[0] = 1;
		PrimPoly[4] = 1;
		PrimPoly[5] = 1;
		PrimPoly[6] = 2;
		
		GaloisField.resize(q);
		for (int i = 0; i < q; i++) 
			GaloisField[i] = i;
	};
	~Galois() {};

	void genRandomSequence(std::vector<int>& GaloisSequence);
	int fold(std::vector<int> GaloisSequence, cv::Mat& FoldedMatrix);

	int q, degree, foldingWidth;
	std::vector<int> GaloisField, PrimPoly;
};


#endif