
#include "doctest.h"

import std;
import utilities;

namespace utils
{
    template <typename T, T lower_bound, T upper_bound>
    doctest::String toString(const constrained<T, lower_bound, upper_bound>& value)
    {
        return std::format("{} [{}, {}]", value.data(), lower_bound, upper_bound).c_str();
    }
}

namespace
{
#define constrained_test_cases \
    utils::constrained<int, 10, 20>, \
    utils::constrained<float, 30.5, 40.5>
}

TEST_CASE_TEMPLATE("utilities.Constrained wrapper work properly within limits", test_case, constrained_test_cases)
{
    test_case value { test_case::min };
    CHECK_EQ(value, test_case::min);

    value += test_case::max - test_case::min;
    CHECK_EQ(value, test_case::max);

    value -= test_case::max - test_case::min;
    CHECK_EQ(value, test_case::min);

    value = test_case::max;
    CHECK_EQ(value, test_case::max);
}