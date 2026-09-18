
export module svg:common;
import std;

namespace svg
{
    export template<typename T>
    using result = std::expected<T, std::string>;

    export using outcome = result<void>;

}