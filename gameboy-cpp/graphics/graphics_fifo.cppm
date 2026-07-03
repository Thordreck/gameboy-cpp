
export module graphics:fifo;

import std;
export import utilities;

import :common;

namespace graphics
{

	export template <std::size_t N>
	requires (N == 8) || (N == 16)
	class pixel_fifo
	{
	public:
		[[nodiscard]] bool try_push(const pixel& pixel) { return queue.try_push(pixel); }

		template<std::ranges::input_range R>
		requires std::convertible_to<std::ranges::range_value_t<R>, pixel>
		[[nodiscard]] bool try_push(R&& range) { return queue.try_push(range); }

		[[nodiscard]] std::optional<pixel> try_pop() { return queue.try_pop(); }
		[[nodiscard]] size_t count() const { return queue.count(); }

		void clear() { queue.clear(); }

	private:
		utils::fixed_size_queue<pixel, N> queue;
	};

	export using tile_row = std::span<const pixel, 8>;
	export using bg_fifo = pixel_fifo<8>;
	export using sprite_fifo = pixel_fifo<8>;

}
