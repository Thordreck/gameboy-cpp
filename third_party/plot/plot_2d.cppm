module;
#include <signalsmith-plot/plot.h>

export module plot:plot_2d;

import :axis;
import :line;
import :internal;

namespace plot
{
    using plot_imp = signalsmith::plot::Plot2D;

    export class plot_2d
    {
    public:
        plot_2d()
            : imp { new plot_imp, std::default_delete<plot_imp>() }
        {}

        explicit plot_2d(plot_imp* external)
            : imp { external, [] (plot_imp*) {} }
        {}

        [[nodiscard]] axis x() { return axis(imp->x); }
        [[nodiscard]] axis y() { return axis(imp->y); }

        [[nodiscard]] const_axis x() const { return const_axis(imp->x); }
        [[nodiscard]] const_axis y() const { return const_axis(imp->y); }

        [[nodiscard]] line_2d line() { return line_2d(imp->line()); }
        [[nodiscard]] line_2d line_fill() { return line_2d(imp->lineFill()); }

        [[maybe_unused]] plot_2d& title(const std::string_view title)
        {
            imp->title( std::string{ title });
            return *this;
        }

    private:
        std::unique_ptr<plot_imp, std::function<void(plot_imp*)>> imp;

        friend struct internal;
        auto* get_imp() { return imp.get(); }
        auto const* get_imp() const { return imp.get(); }
    };

}
