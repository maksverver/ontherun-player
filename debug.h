#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <cstdlib>
#include <iostream>

std::ostream &trace(const char *file, int line);
void abort(const char *file, int line, const char *message);

class ScopeTrace
{
    const char *file;
    const int line;
    const char *msg;
    const int begin;

public:
    ScopeTrace(const char *file, int line, const char *msg);
    ~ScopeTrace();
};

#ifndef NDEBUG
#define ASSERT(EXPR) while(!(EXPR)) { abort(__FILE__,__LINE__, "Assertion failed: "#EXPR); }
#define VERIFY(EXPR) while(!(EXPR)) { abort(__FILE__,__LINE__, "Verification failed: "#EXPR); }
#define TRACE trace(__FILE__, __LINE__)
#define TRACESCOPE(MSG) ScopeTrace _##__LINE__ (__FILE__, __LINE__, (MSG))
#else
#define ASSERT(EXPR)
#define VERIFY(EXPR) (EXPR)
#define TRACE
#define TRACESCOPE(MSG)
#endif

#endif /* ndef DEBUG_H_INCLUDED */
