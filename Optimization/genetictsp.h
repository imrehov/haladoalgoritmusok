#pragma once

#include <cstddef>
#include <limits>
#include <string> // TravellingSalesman.h uses std::string.
#include <vector>

#include "TravellingSalesman.h"
#include "stdafx.h"

class GeneticTravellingSalesman : public TravellingSalesmanProblem
{
public:

    void solve(std::size_t populationSize = 150,
        std::size_t generations = 1500,
        float crossoverRate = 0.9f,
        float mutationRate = 0.2f,
        std::size_t eliteCount = 2);

    float bestDistance() const noexcept;
    std::vector<Town> bestRoute() const;
    void saveBestRoute(const std::string& fileName);

private:
    struct Individual
    {
        std::vector<std::size_t> order;
        float distance;
    };

    float evaluate(const std::vector<std::size_t>& order);
    std::size_t tournamentSelect(const std::vector<Individual>& population) const;
    std::vector<std::size_t> orderCrossover(
        const std::vector<std::size_t>& parentA,
        const std::vector<std::size_t>& parentB) const;
    void mutate(std::vector<std::size_t>& order) const;
    void shuffle(std::vector<std::size_t>& order) const;

    std::vector<std::size_t> bestOrder_;
    float bestDistance_ = std::numeric_limits<float>::infinity();
};

