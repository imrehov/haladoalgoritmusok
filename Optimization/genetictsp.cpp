#include "stdafx.h"
#include "genetictsp.h"
#include "../Utils/Random.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <utility>

void GeneticTravellingSalesman::shuffle(std::vector<std::size_t>& order) const
{
    for (std::size_t i = order.size(); i > 1; --i)
    {
        const auto j = static_cast<std::size_t>(randomUniform(0, static_cast<int>(i - 1)));
        std::swap(order[i - 1], order[j]);
    }
}

float GeneticTravellingSalesman::evaluate(const std::vector<std::size_t>& order)
{
    std::vector<Town> route;
    route.reserve(order.size());
    for (const std::size_t townIndex : order)
        route.push_back(towns[townIndex]);

    return objective(std::move(route));
}

std::size_t GeneticTravellingSalesman::tournamentSelect(
    const std::vector<Individual>& population) const
{
    const int lastIndex = static_cast<int>(population.size() - 1);
    std::size_t best = static_cast<std::size_t>(randomUniform(0, lastIndex));

    for (int i = 1; i < 3; ++i)
    {
        const auto candidate = static_cast<std::size_t>(randomUniform(0, lastIndex));
        if (population[candidate].distance < population[best].distance)
            best = candidate;
    }
    return best;
}

std::vector<std::size_t> GeneticTravellingSalesman::orderCrossover(
    const std::vector<std::size_t>& parentA,
    const std::vector<std::size_t>& parentB) const
{
    const std::size_t n = parentA.size();
    std::size_t left = static_cast<std::size_t>(randomUniform(0, static_cast<int>(n - 1)));
    std::size_t right = static_cast<std::size_t>(randomUniform(0, static_cast<int>(n - 1)));
    if (left > right)
        std::swap(left, right);

    const std::size_t empty = n;
    std::vector<std::size_t> child(n, empty);
    std::vector<bool> used(n, false);

    for (std::size_t i = left; i <= right; ++i)
    {
        child[i] = parentA[i];
        used[parentA[i]] = true;
    }

    std::size_t writePos = (right + 1) % n;
    for (std::size_t offset = 0; offset < n; ++offset)
    {
        const std::size_t gene = parentB[(right + 1 + offset) % n];
        if (!used[gene])
        {
            child[writePos] = gene;
            writePos = (writePos + 1) % n;
        }
    }
    return child;
}

void GeneticTravellingSalesman::mutate(std::vector<std::size_t>& order) const
{
    if (order.size() < 2)
        return;

    const auto first = static_cast<std::size_t>(
        randomUniform(0, static_cast<int>(order.size() - 1)));
    std::size_t second = static_cast<std::size_t>(
        randomUniform(0, static_cast<int>(order.size() - 2)));
    if (second >= first)
        ++second;

    if (randomUniform() < 0.5f)
    {
        std::swap(order[first], order[second]);
    }
    else
    {
        const auto left = std::min(first, second);
        const auto right = std::max(first, second);
        std::reverse(order.begin() + left, order.begin() + right + 1);
    }
}

void GeneticTravellingSalesman::solve(
    std::size_t populationSize,
    std::size_t generations,
    float crossoverRate,
    float mutationRate,
    std::size_t eliteCount)
{
    bestOrder_.clear();
    bestDistance_ = std::numeric_limits<float>::infinity();

    std::vector<std::size_t> baseOrder(towns.size());
    std::iota(baseOrder.begin(), baseOrder.end(), 0);

    std::vector<Individual> population;
    population.reserve(populationSize);
    for (std::size_t i = 0; i < populationSize; ++i)
    {
        auto order = baseOrder;
        if (i > 0)
            shuffle(order);
        const float distance = evaluate(order);
        population.push_back({ std::move(order), distance });
    }

    for (std::size_t generation = 0; generation < generations; ++generation)
    {
        std::sort(population.begin(), population.end(),
            [](const Individual& a, const Individual& b) {
                return a.distance < b.distance;
            });

        if (population.front().distance < bestDistance_)
        {
            bestDistance_ = population.front().distance;
            bestOrder_ = population.front().order;
        }

        std::vector<Individual> nextGeneration;
        nextGeneration.reserve(populationSize);

        for (std::size_t i = 0; i < eliteCount; ++i)
            nextGeneration.push_back(population[i]);

        while (nextGeneration.size() < populationSize)
        {
            const auto& parentA = population[tournamentSelect(population)].order;
            const auto& parentB = population[tournamentSelect(population)].order;

            std::vector<std::size_t> child =
                randomUniform() < crossoverRate
                ? orderCrossover(parentA, parentB)
                : parentA;

            if (randomUniform() < mutationRate)
                mutate(child);

            const float distance = evaluate(child);
            nextGeneration.push_back({ std::move(child), distance });
        }
        population = std::move(nextGeneration);
    }
    for (const auto& individual : population)
    {
        if (individual.distance < bestDistance_)
        {
            bestDistance_ = individual.distance;
            bestOrder_ = individual.order;
        }
    }
}

float GeneticTravellingSalesman::bestDistance() const noexcept
{
    return bestDistance_;
}

std::vector<Town> GeneticTravellingSalesman::bestRoute() const
{
    std::vector<Town> route;
    route.reserve(bestOrder_.size());
    for (const auto index : bestOrder_)
        route.push_back(towns[index]);
    return route;
}

void GeneticTravellingSalesman::saveBestRoute(const std::string& fileName)
{
    saveTownsToFile(fileName, bestRoute());
}