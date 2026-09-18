module;
#include <dsplib.h>

export module dsp:utils;
import :common;

namespace dsp
{
    export base_array<real_t> arange(
        std::convertible_to<float> auto const start,
        std::convertible_to<float> auto const stop,
        std::convertible_to<float> auto const step)
    {
        return dsplib::arange(start, stop, step);
    }

}
