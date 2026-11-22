#pragma once

#include <cstddef>
#include <limits>
#include <string>
#include <vector>

#include "FunctionApproximation.h"
#include "stdafx.h"


class HillClimbingFunctionApproximation : public FunctionApproximation
{
public:

    void solve(std::size_t restarts = 10,
        std::size_t maxIterations = 5000,
        float initialStep = 0.5f,
        float minStep = 0.000001f);

    const std::vector<float>& bestCoefficients() const noexcept;
    float bestError() const noexcept;

private:
    std::vector<float> bestCoefficients_ = std::vector<float>(5, 0.0f);
    float bestError_{ std::numeric_limits<float>::infinity() };
};

