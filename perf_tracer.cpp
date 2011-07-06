#include <vector>
#include <functional>
#include <numeric>

#include "perf_tracer.hpp"

#define RANGE 10

namespace gtc
{

int counter = 0;
static double ms_result[RANGE] = {0.f};
static double us_result[RANGE] = {0.f};

PerfTracer::PerfTracer(bool auto_start)
{
    if (auto_start)
    {
        start();
    }
}

PerfTracer::~PerfTracer(void)
{
    counter = (counter+1) % RANGE;
    ms_result[counter] = ms();
    us_result[counter] = us();
}

void PerfTracer::start(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&start_);
#else
    gettimeofday(&start_, NULL);
#endif
}

void PerfTracer::stop(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&stop_);
#else
    gettimeofday(&stop_, NULL);
#endif
}

double PerfTracer::ms(void)
{
#ifdef WIN32   
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (static_cast<double>(stop_-start_)*1e3f)/static_cast<double>(freq);
#else
    return static_cast<double>((stop_.tv_sec-start_.tv_sec)*1e3f + (stop_.tv_usec-start_.tv_usec)/1e3f);
#endif
}

double PerfTracer::us(void)
{
#ifdef WIN32   
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (static_cast<double>(stop_-start_)*1e6f)/static_cast<double>(freq);
#else
    return static_cast<double>((stop_.tv_sec-start_.tv_sec)*1e6f + (stop_.tv_usec-start_.tv_usec));
#endif
}

double PerfTracer::mean_ms(void)
{
    return std::accumulate(&ms_result[0], &ms_result[RANGE-1], 0, std::plus<double>()) / static_cast<double>(RANGE);
}

double PerfTracer::mean_us(void)
{
    return std::accumulate(&us_result[0], &us_result[RANGE-1], 0, std::plus<double>()) / static_cast<double>(RANGE);
}
} /* namepsace gtc */
