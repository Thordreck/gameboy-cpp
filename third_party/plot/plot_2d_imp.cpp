#include "plot_2d_imp.hpp"
#include <signalsmith-plot/plot.h>

namespace plot
{
    class plot_2d_imp::upstream_plot_2d
    {
    public:
        signalsmith::plot::Plot2D plot {};
    };

    plot_2d_imp::plot_2d_imp()
        : imp { std::make_unique<upstream_plot_2d>() }
    {}

    plot_2d_imp::~plot_2d_imp() = default;

    signalsmith::plot::Plot2D& plot_2d_imp::native() { return imp->plot; }
    const signalsmith::plot::Plot2D& plot_2d_imp::native() const { return imp->plot; }

    signalsmith::plot::Axis& plot_2d_imp::x() { return imp->plot.x; }
    signalsmith::plot::Axis& plot_2d_imp::y() { return imp->plot.y; }

    const signalsmith::plot::Axis& plot_2d_imp::x() const { return imp->plot.x; }
    const signalsmith::plot::Axis& plot_2d_imp::y() const { return imp->plot.y; }

    signalsmith::plot::Line2D& plot_2d_imp::line() { return imp->plot.line(); }

}