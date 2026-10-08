
export module tests.core:common;
import std;

namespace tests
{
    export template<typename T>
    using result = std::expected<T, std::string>;

    using outcome = result<void>;

}