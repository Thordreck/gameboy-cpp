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

    export auto normalized_cross_correlation(SpanLike<real_t> auto&& lhs, SpanLike<real_t> auto&& rhs)
    {
        const std::span lhs_span { lhs.data(), lhs.size() };
        const std::span rhs_span { rhs.data(), rhs.size() };

        const auto lhs_2
            = lhs_span
            | std::views::transform([] (const auto value) { return value * value; })
            | std::ranges::to<std::vector>();

        const auto rhs_2
            = rhs_span
            | std::views::transform([] (const auto value) { return value * value; })
            | std::ranges::to<std::vector>();

        const std::vector<real_t> lhs_ones(lhs.size(), 1.0);
        const std::vector<real_t> rhs_ones(rhs.size(), 1.0);

        const auto lhs_energy = cross_correlation(lhs_2, rhs_ones);
        const auto rhs_energy = cross_correlation(lhs_ones, rhs_2);

        const auto normalization_factors
            = std::views::zip(lhs_energy, rhs_energy)
            | std::views::transform([] (const auto& entry) { return std::sqrt(std::get<0>(entry) * std::get<1>(entry)); })
            | std::ranges::to<std::vector>();

        const auto lhs_array = make_array(lhs.data(), lhs.size(), 1);
        const auto rhs_array = make_array(rhs.data(), rhs.size(), 1);
        const auto correlation = cross_correlation(lhs_array, rhs_array);

        return correlation
            | std::views::enumerate
            | std::views::transform([&normalization_factors] (const auto& zip)
            {
                const auto [k, correlation] = zip;
                const auto normalization = normalization_factors[k];
                const auto normalized = normalization > 0.0f ? correlation / normalization : 0.0f;

                return std::clamp(normalized, -1.0f, 1.0f);
            })
            | std::ranges::to<std::vector>();
    }
}
