#pragma once

#include "tracy/Tracy.hpp"

struct tracy_session_scope
{
    tracy_session_scope() { tracy::StartupProfiler(); }
    ~tracy_session_scope() { tracy::ShutdownProfiler(); }
};

struct tracy_fiber_scope
{
    explicit tracy_fiber_scope(const char* name) { TracyFiberEnter(name); }
    ~tracy_fiber_scope() { TracyFiberLeave; }
};

#define PROFILER_SESSION() SuppressVarShadowWarning( tracy_session_scope TracyConcat(__tracy_source_location,TracyLine){}; )
#define PROFILER_SCOPE(name) ZoneScopedN(name)
#define PROFILER_THREAD(name) SuppressVarShadowWarning( tracy_fiber_scope TracyConcat(__tracy_source_location,TracyLine){ name }; )