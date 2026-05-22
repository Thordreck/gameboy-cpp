module;
#include <boost/lockfree/spsc_queue.hpp>

export module utilities:ring_buffer;
import std;

namespace utils
{
    constexpr std::size_t dynamic_capacity { 0 };

    export template<typename T, size_t Capacity = dynamic_capacity>
    class ring_buffer
    {
    public:
        void clear() { imp.reset(); }

        [[nodiscard]] static constexpr size_t capacity() { return Capacity; }
        [[nodiscard]] size_t write_available() const { return imp.write_available(); }
        [[nodiscard]] size_t read_available() const { return imp.read_available(); }

        [[nodiscard]] bool push(const T& t) { return imp.push(t); }
        [[nodiscard]] bool push(T&& t) { return imp.push(std::forward<T>(t)); }
        [[nodiscard]] size_t push(std::span<const T> buffer) { return imp.push(buffer); }

        [[nodiscard]] std::optional<T> pop() { return imp.pop(boost::lockfree::uses_optional); }
        [[nodiscard]] size_t pop(std::span<T> buffer) { return imp.pop(buffer); }

    private:
        boost::lockfree::spsc_queue<T, boost::lockfree::capacity<Capacity>> imp;
    };

    export template<typename T>
    class ring_buffer<T, dynamic_capacity>
    {
    public:
        explicit ring_buffer(const std::size_t capacity)
            : imp(capacity)
        {}

        void clear() { imp.reset(); }

        [[nodiscard]] size_t capacity() const { return imp.capacity(); }
        [[nodiscard]] size_t write_available() const { return imp.write_available(); }
        [[nodiscard]] size_t read_available() const { return imp.read_available(); }

        [[nodiscard]] bool push(const T& t) { return imp.push(t); }
        [[nodiscard]] bool push(T&& t) { return imp.push(std::forward<T>(t)); }
        [[nodiscard]] size_t push(std::span<const T> buffer) { return imp.push(buffer); }

        [[nodiscard]] std::optional<T> pop() { return imp.pop(boost::lockfree::uses_optional); }
        [[nodiscard]] size_t pop(std::span<T> buffer) { return imp.pop(buffer); }

    private:
        boost::lockfree::spsc_queue<T> imp;
    };

}