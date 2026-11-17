
export module audio:minblep;
import std;

import :utils;
import :window;
import :signals;

namespace audio
{
    export template <size_t ZeroCrossings, size_t OverSampling>
    constexpr size_t min_blep_length { ZeroCrossings * 2 * OverSampling + 1 };

    export template <std::floating_point SampleType, size_t ZeroCrossings, size_t OverSampling>
    using min_blep_t = std::array<SampleType, min_blep_length<ZeroCrossings, OverSampling>>;

    // Adapted from: https://www.experimentalscene.com/articles/minbleps.php
    export template <std::floating_point SampleType, size_t ZeroCrossings, size_t OverSampling>
    requires (ZeroCrossings > 0) && (OverSampling > 0)
    constexpr [[nodiscard]] auto min_blep() noexcept
    {
        constexpr size_t table_length { min_blep_length<ZeroCrossings, OverSampling> };
        constexpr size_t full_table_length { std::bit_ceil(table_length) };

        std::array<SampleType, full_table_length> full_table {};
        std::span<SampleType, table_length> table { full_table.begin(), table_length };

        std::ranges::transform(
            std::views::iota(0uz, table_length),
            table.begin(),
            [] (const size_t i) constexpr
            {
                constexpr auto x = static_cast<SampleType>(2 * ZeroCrossings);
                constexpr auto z = -static_cast<SampleType>(ZeroCrossings);
                const auto y = i / static_cast<SampleType>(table_length - 1);

                const auto fma = math::fma(x, y, z);
                const auto w = blackman_window<SampleType, table_length>(i);

                return sinc(fma) * w;
            });

        rceps(full_table);
        min_phase(full_table);
        integrate(table);
        scale(table, 1 / table.back());

        if constexpr (table_length == full_table_length)
        {
            return full_table;
        }
        else
        {
            return make_array<table_length>([full_table] (const size_t i) constexpr { return full_table[i]; });
        }
    }

}