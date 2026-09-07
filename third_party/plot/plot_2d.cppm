module;
#include "plot_2d_imp.hpp"

export module plot:plot_2d;

import :axis;
import :line;
import :internal;

namespace plot
{
    export class plot_2d
    {
    public:
        [[nodiscard]] axis x() { return axis(imp.x()); }
        [[nodiscard]] axis y() { return axis(imp.y()); }

        [[nodiscard]] const_axis x() const { return const_axis(imp.x()); }
        [[nodiscard]] const_axis y() const { return const_axis(imp.y()); }

        [[nodiscard]] line_2d line() { return line_2d(imp.line()); }

    private:
        plot_2d_imp imp {};

        friend struct internal;
        auto* get_imp() { return &imp.native(); }
        auto const* get_imp() const { return &imp.native(); }
    };

}
