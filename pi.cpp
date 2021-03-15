/*
 * Sean Malloy
 * pi.cpp
 * Multithreaded pi approximation calculator
 */
/****************************************************************************/
// System includes
#include <iostream>
#include <future>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>

/****************************************************************************/
// Local includes
#include "timer.hpp"

/****************************************************************************/
// Forward declarations/using statements

using ull_t = unsigned long long;
using ld_t = long double;

ull_t
partition(int tid, ull_t N, int P);

ld_t
partial_sum_odd_fractions(int tid, ull_t N, int P);

/****************************************************************************/

int 
main(int argc, char* argv[]) 
{
    ull_t N;
    int P;
    if (argc != 3)
    {
        std::cout << "iterations => ";
        std::cin >> N;
        std::cout << "threads    => ";
        std::cin >> P;
    }
    else
    {
        N = std::stoull(argv[1]);
        P = std::stoi(argv[2]);
    }

    Timer timer;
    timer.start();
    std::vector<std::future<ld_t>> threads;
    threads.reserve(P);
    for (int tid = 1; tid < P - 1; ++tid)
        threads.push_back(std::async(std::launch::async, 
                                     partial_sum_odd_fractions, 
                                     tid, N, P));
    
    ld_t sum = partial_sum_odd_fractions(0, N, P);
    
    for (auto& t : threads)
        sum += t.get();
    timer.stop();
    std::cout << std::fixed << std::setprecision(16) 
              << "actual = " << M_PI << '\n'
              << "approx = " << 4 * sum << '\n';
    std::cout << std::fixed << std::setprecision(3) 
              << "time   = " << timer.elapsed() << " ms\n";

    return 0;
}

/****************************************************************************/

ld_t
partial_sum_odd_fractions(int tid, ull_t N, int P)
{
    ull_t begin = partition(tid, N, P);
    ull_t end = partition(tid + 1, N, P);
    ld_t sum = 0.0;
    ld_t k = 2.0 * (begin + 1) - 1.0;

    int sign = (tid + 1) % 2 == 0 ? -1 : 1;

    for (ull_t i = begin; i < end; i += 2)
    {
        sum += sign / k;
        k += 4;
    }

    sign *= -1;
    k = 2.0 * (begin + 1) + 1.0;

    for (ull_t i = begin + 1; i < end; i += 2)
    {
        sum += sign / k;
        k += 4;
    }

    return sum;
}

/****************************************************************************/

ull_t
partition(int tid, ull_t N, int P)
{
    return static_cast<ull_t>(N * tid) / P;
}