module;
#include <dsplib.h>

export module dsp:math;
import :common;

namespace dsp
{
    export auto abs(SpanLike<const real_t> auto const& input)
    {
        return to_vector(dsplib::abs(make_span(input)));
    }

    export auto abs(SpanLike<const complex_t> auto const& input)
    {
        return to_vector(dsplib::abs(make_span(input)));
    }

    export auto pow2db(SpanLike<const real_t> auto const& input)
    {
        return to_vector(dsplib::pow2db(make_span(input)));
    }

    export auto pow2db(SpanLike<const complex_t> auto const& input)
    {
        return to_vector(dsplib::pow2db(make_span(input)));
    }

}