module;
#include <dsplib.h>

export module dsp:correlation;
import :common;

namespace dsp
{
    export auto cross_correlation(SpanLike<const real_t> auto const& lhs, SpanLike<const real_t> auto const& rhs)
    {
        return to_vector(dsplib::xcorr(make_span(lhs), make_span(rhs)));
    }

    export auto cross_correlation(SpanLike<const complex_t> auto const& lhs, SpanLike<const complex_t> auto const& rhs)
    {
        return to_vector(dsplib::xcorr(make_span(lhs), make_span(rhs)));
    }

    export auto auto_correlation(SpanLike<const real_t> auto const& input)
    {
        return to_vector(dsplib::xcorr(make_span(input)));
    }

    export auto auto_correlation(SpanLike<const complex_t> auto const& input)
    {
        return to_vector(dsplib::xcorr(make_span(input)));
    }

    export auto normalized_cross_correlation(SpanLike<const real_t> auto const& lhs, SpanLike<const real_t> auto const& rhs)
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

        const std::vector<real_t> lhs_ones(lhs.size(), real_t { 1.0 });
        const std::vector<real_t> rhs_ones(rhs.size(), real_t { 1.0 });

        const auto overlap_counts = cross_correlation(lhs_ones, rhs_ones);
        const auto min_overlap = lhs.size() * 0.1f;

        const auto lhs_energy = cross_correlation(lhs_2, rhs_ones);
        const auto rhs_energy = cross_correlation(lhs_ones, rhs_2);

        const auto normalization_factors
            = std::views::zip(lhs_energy, rhs_energy)
            | std::views::transform([] (const auto& entry) { return std::sqrt(std::get<0>(entry) * std::get<1>(entry)); })
            | std::ranges::to<std::vector>();

        const auto correlation = cross_correlation(lhs_span, rhs_span);

        return correlation
            | std::views::enumerate
            | std::views::transform([&normalization_factors, &overlap_counts, min_overlap] (const auto& zip)
            {
                const auto [k, correlation] = zip;
                const auto normalization = normalization_factors[k];
                const auto normalized = overlap_counts[k] >= min_overlap && normalization > 0.0f
                    ? correlation / normalization
                    : 0.0f;

                return std::clamp<real_t>(normalized, -1.0f, 1.0f);
            })
            | std::ranges::to<std::vector>();
    }

}
