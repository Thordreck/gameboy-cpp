
export module metadata:common;
import std;

namespace meta
{
    export template<typename T>
    using result = std::expected<T, std::string>;

    export using outcome = result<void>;

}