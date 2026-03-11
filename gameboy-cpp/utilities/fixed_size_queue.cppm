
export module utilities:fixed_size_queue;
export import std;

namespace utils
{
	export template<typename T, size_t N>
	requires (N > 0)
	class fixed_size_queue
	{
	public:
		fixed_size_queue()
			: storage{}
			, head{}
			, tail{}
			, num_elements{}
		{}

		static constexpr size_t size = N;

		bool is_empty() const { return count() == 0; }
		bool is_full() const { return count() == size; }
		size_t count() const { return num_elements; }

		bool try_push(T&& element)
		{
			if (is_full())
			{
				return false;
			}

			storage[tail] = std::forward<T>(element);
			tail = (tail + 1) % size;
			num_elements++;

			return true;
		}

		template<std::ranges::input_range R>
		requires std::convertible_to<std::ranges::range_value_t<R>, T>
		bool try_push(R&& range)
		{
			const auto count = std::ranges::distance(range);

			if ((num_elements + count) > size)
			{
				return false;
			}

			for (auto&& v : range)
			{
				storage[tail] = std::forward<decltype(v)>(v);
				tail = (tail + 1) % size;
			}

			num_elements += count;
			return true;
		}

		template<std::ranges::contiguous_range R>
		requires std::convertible_to<std::ranges::range_value_t<R>, T>
		bool try_push(R&& range)
		{
			const auto count = std::ranges::size(range);

			if ((num_elements + count) > size)
			{
				return false;
			}

			auto* src = std::ranges::data(range);
			const size_t space_at_end = size - tail;

			if (count <= space_at_end)
			{
				std::copy_n(src, count, storage.data() + tail);
			}
			else
			{
				std::copy_n(src, space_at_end, storage.data() + tail);
				std::copy_n(src + space_at_end, count - space_at_end, storage.data());
			}

			tail = (tail + count) % size;
			num_elements += count;
			return true;
		}

		std::optional<T> try_pop()
		{
			if (is_empty())
			{
				return std::nullopt;
			}

			auto element = std::move(storage[head]);
			head = (head + 1) % size;
			num_elements--;

			return element;
		}

	private:
		std::array<T, N> storage;
		size_t head;
		size_t tail;
		size_t num_elements;
	};
}

