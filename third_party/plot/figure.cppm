module;
#include <signalsmith-plot/plot.h>

export module plot:figure;

import std;

import :plot_2d;
import :internal;

namespace plot
{
    export class figure
    {
    public:
        [[nodiscard]] plot_2d plot(const std::size_t column, const std::size_t row)
        {
            auto& plot = imp(column, row).plot();
            return plot_2d { &plot };
        }

    private:
        signalsmith::plot::Figure imp {};

        friend struct internal;
        auto* get_imp() { return &imp; }
        auto const* get_imp() const { return &imp; }
    };

}
