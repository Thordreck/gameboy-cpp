
#pragma once

#if defined(PROFILING_TRACY)
#include "profiling_tracy.hpp"
#elif defined(PROFILING_CTRACK)
#include "profiling_ctrack.hpp"
#elif defined(PROFILING_NONE)
#include "profiling_none.hpp"
#else
#error "No profiling backend configured"
#endif