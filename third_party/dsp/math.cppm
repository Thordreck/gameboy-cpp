module;
#include <dsplib.h>

export module dsp:math;
import :common;

namespace dsp
{
    export base_array<real_t> abs(SpanLike<real_t> auto&& input)
    {
        return dsplib::abs(std::forward<decltype(input)>(input));
    }

    export base_array<real_t> abs(SpanLike<complex_t> auto&& input)
    {
        return dsplib::abs(std::forward<decltype(input)>(input));
    }

    export base_array<real_t> pow2db(SpanLike<real_t> auto&& input)
    {
        return dsplib::pow2db(std::forward<decltype(input)>(input));
    }

    export base_array<real_t> mag2db(SpanLike<real_t> auto&& input)
    {
        return dsplib::mag2db(std::forward<decltype(input)>(input));
    }

}