#ifndef PERF_TRACER_HPP
#define PERF_TRACER_HPP

#include <vector>

#if WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace gtc
{

class PerfTracer
{
private:
#ifdef WIN32
    LARGE_INTEGER start_;
    LARGE_INTEGER stop_;
#else
    struct timeval start_;
    struct timeval stop_;
#endif

public:
    PerfTracer(bool auto_start = true);
    ~PerfTracer(void);
    
    void start(void);
    void stop(void);
    double ms(void);
    double us(void);
    double mean_ms(void);
    double mean_us(void);
};

} /* namespace gtc */

#endif /* PERF_TRACER_HPP */
