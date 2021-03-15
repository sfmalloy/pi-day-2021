#include <iostream>
#include <future>
#include <vector>
#include <iomanip>
#include <limits>

unsigned long long
partition(int tid, unsigned long long N, int P)
{
    return static_cast<unsigned long long>(N) * tid / P;
}

long double
partial_sum(int tid, long long N, int P)
{
    int sign = (tid + 1) % 2 == 0 ? -1 : 1;
    auto begin = partition(tid, N, P);
    auto end = partition(tid + 1, N, P);
    long double sum = 0.0;
    long double k = 2.0 * (begin + 1) - 1.0;

    for (unsigned long long i = begin; i < end; ++i)
    {
        sum += sign / k;
        k += 2;
        sign *= -1;
    }

    return sum;
}

int 
main(int argc, char* argv[]) 
{
    unsigned long long N;
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

    std::vector<std::future<long double>> threads;
    threads.reserve(P);
    for (int tid = 0; tid < P; ++tid)
        threads.push_back(std::async(std::launch::async, partial_sum, tid, N, P));
    
    long double sum = 0.0;
    for (auto& t : threads)
        sum += t.get();
    
    std::cout << std::setprecision(std::numeric_limits<long double>::digits10) << 4 * sum << '\n';

    return 0;
}