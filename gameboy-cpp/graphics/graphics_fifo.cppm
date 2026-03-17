
export module graphics:fifo;
export import :common;
export import std;

import utilities;

namespace graphics
{
	export using tile_row = std::span<const pixel, 8>;

	export class pixel_fifo
	{
	public:
		bool try_push(tile_row row)
		{
			return queue.try_push(row);
		}

		std::optional<pixel> try_pop()
		{
			return queue.try_pop();
		}

		void clear()
		{
			queue.clear();
		}

	private:
		utils::fixed_size_queue<pixel, 16> queue;
	};
}
