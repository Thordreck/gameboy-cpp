module;
#include <dsplib.h>

export module dsp:common;
import std;

namespace dsp
{
    export using real_t = dsplib::real_t;
    export using complex_t = dsplib::cmplx_t;

    export template<typename Span, typename Type>
    concept SpanLike = requires(Span span)
    {
        { span.data() } -> std::convertible_to<Type*>;
        { span.size() } -> std::convertible_to<size_t>;
    };

    export template<typename Slice, typename Type>
    concept SliceLike = requires(Slice span)
    {
        { span.data() } -> std::convertible_to<Type*>;
        { span.size() } -> std::convertible_to<size_t>;
        { span.stride() } -> std::convertible_to<size_t>;
    };

}