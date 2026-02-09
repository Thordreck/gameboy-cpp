
#include "doctest.h"

import utilities;
import std;

TEST_CASE_TEMPLATE("add overflows are properly detected", T, std::uint8_t, float)
{
	CHECK_FALSE(utils::check_add_overflow<T>({}, {}));
	CHECK_FALSE(utils::check_add_overflow<T>(std::numeric_limits<T>::max(), {}));
	CHECK(utils::check_add_overflow<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max()));
}

TEST_CASE("half add overflows are properly detected")
{
	CHECK_FALSE(utils::check_half_add_overflow({}, {}));
	CHECK(utils::check_half_add_overflow(15, 1));
}
