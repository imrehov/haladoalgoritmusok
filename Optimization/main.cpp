#include "stdafx.h"

#include "kmeansimg.h"
#include "hillclimbfunc.h"
#include "genetictsp.h"
#include "../Utils/TextLog.h"

#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using Clock = std::chrono::steady_clock;

static double elapsedMilliseconds(Clock::time_point start)
{
    return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

int main()
{
    try
    {
        std::ofstream csv("Log/results.csv");
        if (!csv)
        {
            std::cerr << "Cannot open Log/results.csv. Check Working Directory.\n";
            return 1;
        }
        csv << "algorithm;parameter;run;score;time_ms\n";
        csv << std::fixed << std::setprecision(6);

        Textlog log("Log/runlog.txt");
        constexpr int repetitions = 5;

        // 1) K-MEANS IMAGE SEGMENTATION
        for (int k : {2, 3, 5})
        {
            for (int run = 1; run <= repetitions; ++run)
            {
                KMeansImage image(k);
                image.loadImageFromFile("Input/slide_sample.bmp");

                const auto start = Clock::now();
                image.segment();
                const double ms = elapsedMilliseconds(start);

                if (run == 1)
                    image.savePixClusterToFile("Log/KMeans_K" + std::to_string(k) + ".bmp");

                csv << "KMeans;K=" << k << ';' << run << ";NA;" << ms << '\n';
                log.putInfo("KMeans K=" + std::to_string(k) +
                    " run=" + std::to_string(run) +
                    " time_ms=" + std::to_string(ms));
                std::cout << "KMeans K=" << k << " run=" << run
                    << " time=" << ms << " ms\n";
            }
        }

        // 2) FUNCTION APPROXIMATION: HILL CLIMBING
        for (int restarts : {1, 5, 10})
        {
            for (int run = 1; run <= repetitions; ++run)
            {
                HillClimbingFunctionApproximation hc;
                hc.loadKnownValuesFromFile("Input/FuncAppr1.txt");

                const auto start = Clock::now();
                hc.solve(restarts, 5000, 0.5f, 0.000001f);
                const double ms = elapsedMilliseconds(start);

                csv << "HillClimbing;restarts=" << restarts << ';' << run
                    << ';' << hc.bestError() << ';' << ms << '\n';

                std::string info = "HillClimbing restarts=" + std::to_string(restarts) +
                    " run=" + std::to_string(run) +
                    " SSE=" + std::to_string(hc.bestError()) +
                    " time_ms=" + std::to_string(ms) +
                    " coefficients=";
                for (float c : hc.bestCoefficients())
                    info += std::to_string(c) + " ";
                log.putInfo(info);
                std::cout << "HillClimbing restarts=" << restarts << " run=" << run
                    << " SSE=" << hc.bestError() << " time=" << ms << " ms\n";
            }
        }

        // 3) TRAVELLING SALESMAN: GENETIC ALGORITHM
        for (int generations : {250, 500, 1500})
        {
            for (int run = 1; run <= repetitions; ++run)
            {
                GeneticTravellingSalesman tsp;
                tsp.loadTownsFromFile("Input/Towns.txt");

                const auto start = Clock::now();
                tsp.solve(100, generations, 0.9f, 0.2f, 2);
                const double ms = elapsedMilliseconds(start);

                if (run == 1)
                    tsp.saveBestRoute("Log/TSP_G" + std::to_string(generations) + ".txt");

                csv << "GeneticTSP;generations=" << generations << ';' << run
                    << ';' << tsp.bestDistance() << ';' << ms << '\n';
                log.putInfo("GeneticTSP generations=" + std::to_string(generations) +
                    " run=" + std::to_string(run) +
                    " distance=" + std::to_string(tsp.bestDistance()) +
                    " time_ms=" + std::to_string(ms));
                std::cout << "GeneticTSP generations=" << generations << " run=" << run
                    << " distance=" << tsp.bestDistance() << " time=" << ms << " ms\n";
            }
        }

        std::cout << "\nResults saved in Log/results.csv and Log/runlog.txt\n";
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

