#include "debug.h"
#include <cstdlib>
#include <iomanip>
#include <sys/time.h>

static bool initialized;
static timeval tv_begin, tv_end;

static int timer()
{
    if(!initialized)
    {
        initialized = true;
        gettimeofday(&tv_begin, NULL);
    }
    gettimeofday(&tv_end, NULL);
    return 1000000*(tv_end.tv_sec  - tv_begin.tv_sec) +
                   (tv_end.tv_usec - tv_begin.tv_usec);
}

std::ostream &add_time(std::ostream &os)
{
    int us = timer();
    std::streamsize w = os.width();
    char f = os.fill();
    return os << std::setfill('0') << std::setw(2) << us/1000000 << '.'
              << std::setw(3) << (us/1000)%1000
              << std::setfill(f) << std::setw(w);
}

std::ostream &trace(const char *file, int line)
{
    return add_time(std::cerr) << " [" << file << ':' << line << "] ";
}

void abort(const char *file, int line, const char *message)
{
    trace(file, line) << message;
    std::abort();
}

ScopeTrace::ScopeTrace(const char *file, int line, const char *msg)
    : file(file), line(line), msg(msg), begin(timer())
{
    trace(file, line) << "{ " << msg << std::endl;
}

ScopeTrace::~ScopeTrace() {
    trace(file, line) << "} took: " << (timer() - begin)/1000 << "ms" << std::endl;
}
