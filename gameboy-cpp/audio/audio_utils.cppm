
export module audio:utils;

import std;
import math;

namespace audio
{
    export template<typename SampleType>
    requires std::is_floating_point_v<SampleType>
    constexpr [[nodiscard]] SampleType sinc(const SampleType x) noexcept
    {
        using namespace std::numbers;

        return x == SampleType {}
            ? static_cast<SampleType>(1)
            : math::sin(pi_v<SampleType> * x) / (pi_v<SampleType> * x);
    }

}