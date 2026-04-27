#pragma once

#include "ctrack.hpp"

struct ctrack_session_scope
{
    ~ctrack_session_scope()
    {
        constexpr ctrack::ctrack_result_settings settings { 2, 0.5, 1.0 };
        ctrack::result_print(settings);
    }
};

#define PROFILER_SESSION() ctrack_session_scope ctrack_scope {};
#define PROFILER_SCOPE(name) CTRACK_NAME(name)
#define PROFILER_THREAD(name)
