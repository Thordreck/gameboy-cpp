module;
#include <signalsmith-plot/plot.h>

export module plot:svg;

import std;
import :common;
import :internal;

namespace plot
{
    export template <typename Plot>
    concept SvgExportable = WrapperFor<signalsmith::plot::SvgFileDrawable, Plot>;

    export template<SvgExportable Plot>
    outcome_t save_as_svg(Plot& plot, const std::filesystem::path& path)
    {
        try
        {
            auto* svg_exportable = internal::get_imp(plot);
            svg_exportable->write(path.string());

            return {};
        }
        catch (const std::exception& ex)
        {
            return std::unexpected{ ex.what() };
        }
    }

}