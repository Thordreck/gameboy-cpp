#pragma once

#include <memory>
#include <signalsmith-plot/plot.h>

namespace plot
{
    class plot_2d_imp
    {
    public:
        plot_2d_imp();
        ~plot_2d_imp();

        [[nodiscard]] signalsmith::plot::Plot2D& native();
        [[nodiscard]] const signalsmith::plot::Plot2D& native() const;

        [[nodiscard]] signalsmith::plot::Axis& x();
        [[nodiscard]] signalsmith::plot::Axis& y();

        [[nodiscard]] const signalsmith::plot::Axis& x() const;
        [[nodiscard]] const signalsmith::plot::Axis& y() const;

        [[nodiscard]] signalsmith::plot::Line2D& line();

    private:
        class upstream_plot_2d;
        std::unique_ptr<upstream_plot_2d> imp;
    };

}
