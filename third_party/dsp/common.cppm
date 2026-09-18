module;
#include <dsplib.h>

export module dsp:common;
import std;

namespace dsp
{
    export using real_t = dsplib::real_t;
    export using complex_t = dsplib::cmplx_t;

    export template<typename Type>
    concept RealOrComplex = std::convertible_to<Type, real_t> || std::convertible_to<Type, complex_t>;

    export template<typename Type>
    using base_array = dsplib::base_array<Type>;

    export template<typename Type>
    concept WithValueType = requires
    {
        typename Type::value_type;
    };

    export template<typename Span, typename Type>
    concept SpanLike = requires(Span span)
    {
        { span.data() } -> std::convertible_to<const Type*>;
        { span.size() } -> std::convertible_to<size_t>;
    };

    export template<typename Slice, typename Type>
    concept SliceLike = requires(Slice span)
    {
        { span.data() } -> std::convertible_to<Type*>;
        { span.size() } -> std::convertible_to<size_t>;
        { span.stride() } -> std::convertible_to<size_t>;
    };

    export base_array<complex_t> make_array(const complex_t* data, const size_t size, const size_t stride)
    {
        const auto slice = dsplib::slice_t<complex_t>::make_slice(data, size, 0, size, stride);
        return base_array<complex_t> { slice };
    }

    export base_array<real_t> make_array(const real_t* data, const size_t size, const size_t stride)
    {
        const auto slice = dsplib::slice_t<real_t>::make_slice(data, size, 0, size, stride);
        return base_array<real_t> { slice };
    }

}