module;
#include <dsplib.h>

export module dsp:correlation;
import :common;

namespace dsp
{
    export auto cross_correlation(SpanLike<complex_t> auto&& lhs, SpanLike<complex_t> auto&& rhs)
    {
        return dsplib::xcorr(std::forward<decltype(lhs)>(lhs), std::forward<decltype(rhs)>(rhs));
    }

    export auto cross_correlation(SpanLike<real_t> auto&& lhs, SpanLike<real_t> auto&& rhs)
    {
        return dsplib::xcorr(std::forward<decltype(lhs)>(lhs), std::forward<decltype(rhs)>(rhs));
    }

    export auto auto_correlation(SpanLike<complex_t> auto&& input)
    {
        return dsplib::xcorr(std::forward<decltype(input)>(input));
    }

    export auto auto_correlation(SpanLike<real_t> auto&& input)
    {
        return dsplib::xcorr(std::forward<decltype(input)>(input));
    }
}
