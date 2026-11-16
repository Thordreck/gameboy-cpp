
export module audio:signals;

import std;
import math;

import :dft;
import :fft;
import :utils;

namespace audio
{
    export template<std::floating_point SampleType>
    constexpr [[nodiscard]] SampleType sinc(const SampleType x) noexcept
    {
        using namespace std::numbers;

        return x == SampleType {}
            ? static_cast<SampleType>(1)
            : math::sin(pi_v<SampleType> * x) / (pi_v<SampleType> * x);
    }

    export template <FloatingPointStaticRange Input, RealFFTRange Output>
    requires SameSizeStaticRanges<Input, Output>
    constexpr void rceps(const Input& input, Output& output)
    {
        const auto transform = fft(input);
        const auto log_cabs = make_array<transform.size()>([&transform] (const size_t i) constexpr
        {
            return std::complex { math::log(math::cabs(transform[i])) };
        });

        ifft(log_cabs, output);
    }

    export template <RealFFTRange Range>
    constexpr void rceps(Range& range)
    {
        rceps(range, range);
    }

    export template <RealFFTRange Input, RealFFTRange Output>
    requires SameSizeStaticRanges<Input, Output>
    constexpr void min_phase(const Input& input, Output& output)
    {
        using sample_type = std::ranges::range_value_t<Input>;
        constexpr auto n { std::ranges::size(input) };

        std::ranges::transform(input | std::views::enumerate, output.begin(), [] (auto&& entry) constexpr
        {
            const auto [index, element] = entry;

            if (index == 0)
            {
                return element;
            }

            const auto nd2 = n / 2;

            if (index < nd2)
            {
                return 2 * element;
            }

            if (n % 2 == 0 && index == nd2)
            {
                return element;
            }

            return sample_type {};
        });

        auto transform = fft(output);

        std::ranges::transform(transform.begin(), transform.end(), transform.begin(), [] (auto&& element) constexpr
        {
            return math::cexp(element);
        });

        ifft(transform, output);
    }

    export template <RealFFTRange Range>
    constexpr void min_phase(Range& range)
    {
        min_phase(range, range);
    }

}