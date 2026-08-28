module;
#include <boost/lockfree/spsc_value.hpp>

export module utilities:triple_buffer;
import std;

namespace utils
{
    export template<typename T>
    class triple_buffer
    {
    public:
        void write(T&& value) { imp.write(std::move(value)); }
        void write(const T& value) { imp.write(value); }

        [[nodiscard]] T read() { return imp.read(boost::lockfree::uses_optional).value(); }

    private:
        boost::lockfree::spsc_value<T, boost::lockfree::allow_multiple_reads<true>> imp {};
    };

}
