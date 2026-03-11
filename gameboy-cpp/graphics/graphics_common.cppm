
export module graphics:common;
export import std;

namespace graphics
{
	export using color_channel_t = std::uint8_t;

	export struct color
	{
		color_channel_t r;
		color_channel_t g;
		color_channel_t b;
		color_channel_t a;
	};

	export using coords_2d_axis_t = std::uint8_t;

	export struct coords_2d
	{
		coords_2d_axis_t x;
		coords_2d_axis_t y;
	};
}
