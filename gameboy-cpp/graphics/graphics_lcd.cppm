
export module graphics:lcd;

export import std;
export import :common;

namespace graphics
{
	export constexpr std::uint8_t lcd_width = 160;
	export constexpr std::uint8_t lcd_height = 144;
	export constexpr std::size_t lcd_num_pixels = lcd_width * lcd_height;
	export constexpr std::size_t lcd_memory_size = lcd_num_pixels * num_color_channels;

	export using lcd_memory_span_t = std::span<std::uint8_t, lcd_memory_size>;
	export using lcd_memory_view_t = std::span<const std::uint8_t, lcd_memory_size>;
	export using lcd_scanline_span_t = std::span<const std::uint8_t, lcd_width * num_color_channels>;
	export using lcd_memory_t = std::array<std::uint8_t, lcd_memory_size>;

	export template<typename T>
	concept LCD = requires(T& lcd, const std::uint8_t scanline_idx, const lcd_scanline_span_t scanline)
	{
		{ lcd.write_scanline(scanline_idx, scanline) } -> std::same_as<void>;
		{ lcd.end_frame() } -> std::same_as<void>;
	};

	export template <typename T>
	concept FramebufferSink = requires(T& sink, const lcd_memory_view_t memory)
	{
		{ sink.write_frame(memory) } -> std::same_as<void>;
	};

	export template <FramebufferSink Sink>
	class lcd_framebuffer_sink_adapter
	{
	public:
		lcd_framebuffer_sink_adapter(Sink& sink, const lcd_memory_span_t memory)
			: sink { sink }
			, memory { memory }
		{}

		void write_scanline(const std::uint8_t scanline_idx, const lcd_scanline_span_t scanline)
		{
			const std::size_t pixel_data_pos = scanline_idx * lcd_width * num_color_channels;
			std::ranges::copy(scanline, memory.begin() + pixel_data_pos);
		}

		void end_frame()
		{
			sink.write_frame(memory);
		}

	private:
		Sink& sink;
		lcd_memory_span_t memory;
	};

}
