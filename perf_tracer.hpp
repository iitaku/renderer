#ifndef PERF_TRACER_HPP
#define PERF_TRACER_HPP

#if WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

extern std::map<const char*, double>

class PerfTracer
{
private:
    const char* key_

#ifdef WIN32
    LARGE_INTEGER start_;
    LARGE_INTEGER stop_;
#else
    struct timeval start_;
    struct timeval stop_;
#endif

public:
    PerfTracer(const char* key)
        : key_(key)
    {
#ifdef WIN32
        QueryPerformanceCounter(&start);
#else
        gettimeofday(&start, NULL);
#endif
    }

    ~PerfTracer(void)
    {
#ifdef WIN32
        LARGE_INTEGER freq;
        QueryPerformanceCounter(&stop);
        QueryPerformanceFrequency(&freq);


#else
        gettimeofday(&stop, NULL);
#endif
    }
    
};

#endif /* PERF_TRACER_HPP */
