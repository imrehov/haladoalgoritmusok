#include "stdafx.h"
#include "kmeansimg.h"
#include "../Utils/Random.h"



KMeansImage::KMeansImage(const int initialK)
	: K{ initialK }
	, sums(initialK, 0)
	, counts(initialK, 0)
{
	centroids.reserve(static_cast<size_t>(K));

	for (std::size_t i{ 0 }; i < K; ++i)
	{
		centroids.push_back(randomUniform(0, 255));
	}

	

}


void KMeansImage::segment()
{
	while (true)
	{
		auto oldCentroids{ centroids };
		std::fill(sums.begin(), sums.end(), 0);
		std::fill(counts.begin(), counts.end(), 0);

		for (unsigned int y{ 0 }; y < height; ++y)
		{
			for (unsigned int x{ 0 }; x < width; ++x)
			{

				int pixel = pixg(x, y);

				int bestIndex{ 0 };
				int bestDistance{ static_cast<int>(abs(centroids[0] - pixel)) };

				for (std::size_t i{ 1 }; i < centroids.size(); ++i)
				{

					int distance{ static_cast<int>(abs(centroids[i] - pixel)) };

					if (distance < bestDistance)
					{
						bestDistance = distance;
						bestIndex = i;
					}

				}

				sums[bestIndex] += pixel;
				++counts[bestIndex];

				setPixCluster(x, y, bestIndex);
			}
		}
		int check = K;
		for (std::size_t k{ 0 }; k < K; ++k)
		{
			if (counts[k] > 0)
				centroids[k] = sums[k] / counts[k];

			if (oldCentroids[k] == centroids[k])
				--check;
		}

		if (check == 0)
			break;

	}

	
}