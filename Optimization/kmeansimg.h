#pragma once
#include <vector>

#include "ImageSegmentation.h"
#include "stdafx.h"

class KMeansImage : public ImageSegmentation
{

public:
	void segment();
	KMeansImage(const int initialK);

private:
	
	std::vector<int> centroids;
	std::vector<uint64_t> sums;
	std::vector<size_t> counts;
	int K;
};