module;
#include <dsplib.h>

export module dsp:window;

import std;
import :common;

namespace dsp
{
    export auto hamming(const size_t n)
    {
        return to_vector(dsplib::window::hamming(static_cast<int>(n)));
    }

}
