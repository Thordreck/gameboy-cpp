
export module utilities:fixed_capacity_vector;
export import std;

namespace utils
{
    export template<typename T, size_t N>
    requires (N > 0) && std::destructible<T>
    class fixed_capacity_vector
    {
    public:
        ~fixed_capacity_vector() { clear(); }

        [[nodiscard]] static constexpr size_t capacity() { return N; }
        [[nodiscard]] size_t size() const { return num_elements; }
        [[nodiscard]] bool empty() const { return num_elements == 0; }
        [[nodiscard]] bool is_full() const { return size() == capacity(); }

        [[nodiscard]] T* begin() { return reinterpret_cast<T*>(elements.data()); }
        [[nodiscard]] T* end() { return reinterpret_cast<T*>(elements.data() + num_elements + 1); }

        [[nodiscard]] const T* begin() const { return reinterpret_cast<const T*>(elements.data()); }
        [[nodiscard]] const T* end() const { return reinterpret_cast<const T*>(elements.data() + num_elements + 1); }

        [[nodiscard]] T& operator[] (const std::size_t n) { return *reinterpret_cast<T*>(elements[n].data()); }
        [[nodiscard]] const T& operator[] (const std::size_t n) const { return *reinterpret_cast<const T*>(elements[n].data()); }

        void push_back(const T& element) { new(elements[num_elements++].data()) T(element); }
        void push_back(T&& element) { new(elements[num_elements++].data()) T(std::move(element)); }

        void clear()
        {
            std::ranges::for_each(elements | std::views::take(num_elements),
                                  [](auto& element) { std::destroy_at(element.data()); });
            num_elements = 0;
        }

    private:
        size_t num_elements {};

        using storage_t = std::array<std::byte, sizeof(T)>;
        alignas(alignof(T)) std::array<storage_t, N> elements {};
    };

}