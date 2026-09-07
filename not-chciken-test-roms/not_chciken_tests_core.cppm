module;
#include "doctest.h"

export module not_chciken;
import std;
import plot;

namespace not_chciken
{
	template<typename TExpected, typename TError>
	TExpected require_success(std::expected<TExpected, TError>&& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return std::forward<TExpected>(result.value());
	}

	[[nodiscard]] std::expected<std::filesystem::path, std::string> export_plot(
		plot::plot_2d& plot,
		const std::string_view filename)
	{
		const std::filesystem::path output_filepath
			= std::filesystem::temp_directory_path()
			/ std::filesystem::path(filename).filename().replace_extension("svg");

		const auto result = plot::save_as_svg(plot, output_filepath);
		return result.transform([&output_filepath] { return output_filepath; });
	}

    export void run_square_preset_test(const std::uint8_t preset_idx)
    {
        // TODO: testing plot library
        using namespace plot;
        plot_2d plot {};

        plot.x()
            .majors(tick{0}, tick {10})
            .tick(tick{5})
            .minor(tick {3.5})
            .linear(0, 15);

        plot.y()
            .major(tick{0});

		plot.line()
			.add(std::array { 0, 1, 2, 3 }, std::array { 0, 1, 2, 3, 4 });

		const auto image_path = require_success(export_plot(plot, "plot_test"));
		FAIL(std::format("Generated result image at {}", image_path.string()));
    }

}