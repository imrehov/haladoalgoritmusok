#include "stdafx.h"
#include "hillclimbfunc.h"
#include "../Utils/Random.h"

#include <cmath>
#include <utility>

void HillClimbingFunctionApproximation::solve(
    std::size_t restarts,
    std::size_t maxIterations,
    float initialStep,
    float minStep)
{
    bestError_ = std::numeric_limits<float>::infinity();
    bestCoefficients_.assign(5, 0.0f);

    for (std::size_t restart = 0; restart < restarts; ++restart)
    {
        std::vector<float> current(5, 0.0f);

        if (restart != 0)
        {
            for (float& coefficient : current)
                coefficient = randomUniform(-1.0f, 1.0f);
        }

        float currentError = objective(current);
        float step = initialStep;

        for (std::size_t iteration = 0; iteration < maxIterations && step >= minStep; ++iteration)
        {
            std::vector<float> next = current;
            float nextError = currentError;

            for (std::size_t i = 0; i < current.size(); ++i)
            {
                for (float direction : {-1.0f, 1.0f})
                {
                    std::vector<float> candidate = current;
                    candidate[i] += direction * step;

                    const float candidateError = objective(candidate);
                    if (candidateError < nextError)
                    {
                        next = std::move(candidate);
                        nextError = candidateError;
                    }
                }
            }

            if (nextError < currentError)
            {
                current = std::move(next);
                currentError = nextError;
            }
            else
            {
                step *= 0.5f;
            }
        }

        if (currentError < bestError_)
        {
            bestError_ = currentError;
            bestCoefficients_ = std::move(current);
        }
    }
}

const std::vector<float>& HillClimbingFunctionApproximation::bestCoefficients() const noexcept
{
    return bestCoefficients_;
}

float HillClimbingFunctionApproximation::bestError() const noexcept
{
    return bestError_;
}