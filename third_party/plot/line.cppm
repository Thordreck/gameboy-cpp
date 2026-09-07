module;
#include <signalsmith-plot/plot.h>

export module plot:line;
import :common;
import :internal;

namespace plot
{
    using line_imp = signalsmith::plot::Line2D;

    template <typename Wrapper>
    concept WrapperForLine2D = WrapperFor<line_imp, Wrapper>;

    void add_point(WrapperForLine2D auto& line, const double x, const double y)
    {
        internal::get_imp(line)->add(x, y);
    }

    void add_array(WrapperForLine2D auto& line, Indexable<double> auto&& x, Indexable<double> auto&& y, const size_t size)
    {
        internal::get_imp(line)->addArray(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), size);
    }

    void add_array(WrapperForLine2D auto& line, IndexableWithSize<double> auto&& x, IndexableWithSize<double> auto&& y)
    {
        internal::get_imp(line)->addArray(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
    }

    export class line_2d
    {
    public:
        explicit line_2d(line_imp& imp)
            : imp { imp }
        {}

        [[maybe_unused]] line_2d& add(const double x, const double y)
        {
            add_point(*this, x, y);
            return *this;
        }

        [[maybe_unused]] line_2d& add(Indexable<double> auto&& x, Indexable<double> auto&& y, const size_t size)
        {
            add_array(*this, std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), size);
            return *this;
        }

        [[maybe_unused]] line_2d& add(IndexableWithSize<double> auto&& x, IndexableWithSize<double> auto&& y)
        {
            add_array(*this, std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
            return *this;
        }

    private:
        line_imp& imp;

        friend struct internal;
        auto* get_imp() { return &imp; }
        auto const* get_imp() const { return &imp; }
    };

}
