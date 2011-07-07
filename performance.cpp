#include <vector>
#include <functional>
#include <numeric>

#include "performance.hpp"

#define RANGE 10

namespace gtc
{

int counter = 0;
static double ms_result[RANGE] = {0.f};
static double us_result[RANGE] = {0.f};

Performance::Performance(bool auto_start)
{
    if (auto_start)
    {
        start();
    }
}

Performance::~Performance(void)
{
    counter = (counter+1) % RANGE;
    ms_result[counter] = ms();
    us_result[counter] = us();
}

void Performance::start(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&start_);
#else
    gettimeofday(&start_, NULL);
#endif
}

void Performance::stop(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&stop_);
#else
    gettimeofday(&stop_, NULL);
#endif
}

double Performance::ms(void)
{
#ifdef WIN32   
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (static_cast<double>(stop_-start_)*1e3f)/static_cast<double>(freq);
#else
    return static_cast<double>((stop_.tv_sec-start_.tv_sec)*1e3f + (stop_.tv_usec-start_.tv_usec)/1e3f);
#endif
}

double Performance::us(void)
{
#ifdef WIN32   
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (static_cast<double>(stop_-start_)*1e6f)/static_cast<double>(freq);
#else
    return static_cast<double>((stop_.tv_sec-start_.tv_sec)*1e6f + (stop_.tv_usec-start_.tv_usec));
#endif
}

double Performance::mean_ms(void)
{
    return std::accumulate(&ms_result[0], &ms_result[RANGE-1], 0, std::plus<double>()) / static_cast<double>(RANGE);
}

double Performance::mean_us(void)
{
    return std::accumulate(&us_result[0], &us_result[RANGE-1], 0, std::plus<double>()) / static_cast<double>(RANGE);
}
} /* namepsace gtc */
