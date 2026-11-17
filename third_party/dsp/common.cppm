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
    concept WithValueType = requires
    {
        typename Type::value_type;
    };

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

    auto make_span(const real_t* data, const size_t size)
    {
        return dsplib::make_span(data, size);
    }

    auto make_span(const complex_t* data, const size_t size)
    {
        return dsplib::make_span(data, size);
    }

    auto make_span(SpanLike<const real_t> auto const& input)
    {
        return make_span(input.data(), input.size());
    }

    auto make_span(SpanLike<const complex_t> auto const& input)
    {
        return make_span(input.data(), input.size());
    }

    auto make_slice(const real_t* data, const size_t size, const size_t stride)
    {
        return dsplib::slice_t<real_t>::make_slice(data, static_cast<int>(size), 0, static_cast<int>(size), static_cast<int>(stride));
    }

    auto make_slice(const complex_t* data, const size_t size, const size_t stride)
    {
        return dsplib::slice_t<complex_t>::make_slice(data, static_cast<int>(size), 0, static_cast<int>(size), static_cast<int>(stride));
    }

    auto make_slice(SliceLike<const real_t> auto const& input)
    {
        return make_slice(input.data(), input.size(), input.stride());
    }

    auto make_slice(SliceLike<const complex_t> auto const& input)
    {
        return make_slice(input.data(), input.size(), input.stride());
    }

    auto make_array(const real_t* data, const size_t size, const size_t stride)
    {
        const auto slice = make_slice(data, size, stride);
        return dsplib::base_array<real_t> { slice };
    }

    auto make_array(const complex_t* data, const size_t size, const size_t stride)
    {
        const auto slice = make_slice(data, size, stride);
        return dsplib::base_array<complex_t> { slice };
    }

    auto make_array(const real_t* data, const size_t size)
    {
        return make_array(data, size, 1);
    }

    auto make_array(const complex_t* data, const size_t size)
    {
        return make_array(data, size, 1);
    }

    auto make_array(SpanLike<const real_t> auto const& input)
    {
        return make_array(input.data(), input.size());
    }

    auto make_array(SpanLike<const complex_t> auto const& input)
    {
        return make_array(input.data(), input.size());
    }

    auto make_array(SliceLike<const real_t> auto const& input)
    {
        return make_array(input.data(), input.size(), input.stride());
    }

    auto make_array(SliceLike<const complex_t> auto const& input)
    {
        return make_array(input.data(), input.size(), input.stride());
    }

    auto to_vector(const dsplib::base_array<real_t>& input)
    {
        return std::vector(input.begin(), input.end());
    }

    auto to_vector(const dsplib::base_array<complex_t>& input)
    {
        return std::vector(input.begin(), input.end());
    }

}