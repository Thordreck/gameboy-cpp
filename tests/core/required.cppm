module;
#include "doctest.h"

export module tests.core:required;
import std;

namespace tests
{
    export template<typename TExpected, typename TError>
    [[nodiscard]] TExpected required(std::expected<TExpected, TError>&& result)
    {
        REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
        return std::forward<TExpected>(result.value());
    }

    export template<typename TError>
    void required(std::expected<void, TError>&& result)
    {
        REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
    }

}