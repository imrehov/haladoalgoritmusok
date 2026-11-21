#pragma once
#include <vector>

#include "ImageSegmentation.h"

class KMeansImage : ImageSegmentation
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