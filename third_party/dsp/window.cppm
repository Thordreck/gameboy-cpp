module;
#include <dsplib.h>

export module dsp:window;

import std;
import :common;

namespace dsp
{
    export base_array<real_t> hamming(const size_t n)
    {
        return dsplib::window::hamming(n);
    }

}
