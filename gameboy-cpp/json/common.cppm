
export module json:common;
import std;

namespace json
{
    export template<typename T>
    using result =  std::expected<T, std::string>;

    export using outcome = result<void>;
    }