module;
#include <signalsmith-plot/plot.h>

export module plot:axis;

import std;

import :tick;
import :common;
import :internal;

namespace plot
{
    using axis_imp = signalsmith::plot::Axis;
    using tick_imp = signalsmith::plot::Tick;

    template <typename Wrapper>
    concept WrapperForAxis = WrapperFor<axis_imp, Wrapper>;

    template <typename Wrapper>
    concept WrapperForConstAxis = WrapperFor<const axis_imp, const Wrapper>;

    template <typename Wrapper>
    concept WrapperForTick = WrapperFor<tick_imp, Wrapper>;

    double axis_draw_min(WrapperForConstAxis auto const& wrapper)
    {
        return internal::get_imp(wrapper)->drawMin();
    }

    double axis_draw_max(WrapperForConstAxis auto const& wrapper)
    {
        return internal::get_imp(wrapper)->drawMax();
    }

    double axis_draw_size(WrapperForConstAxis auto const& wrapper)
    {
        return internal::get_imp(wrapper)->drawSize();
    }

    void axis_set_major(WrapperForAxis auto& axis, WrapperForTick auto tick)
    {
        internal::get_imp(axis)->major(*internal::get_imp(tick));
    }

    void axis_set_minor(WrapperForAxis auto& axis, WrapperForTick auto tick)
    {
        internal::get_imp(axis)->minor(*internal::get_imp(tick));
    }

    void axis_set_tick(WrapperForAxis auto& axis, WrapperForTick auto tick)
    {
        internal::get_imp(axis)->tick(*internal::get_imp(tick));
    }

    void axis_set_linear(WrapperForAxis auto& axis, const double low, const double high)
    {
        internal::get_imp(axis)->linear(low, high);
    }

    void axis_set_range(WrapperForAxis auto& axis, IsInvocableR<double, double> auto&& map)
    {
        internal::get_imp(axis)->range(std::forward<decltype(map)>(map));
    }

    void axis_set_range(
        WrapperForAxis auto& axis,
        IsInvocableR<double, double> auto&& map,
        const double low,
        const double high)
    {
        internal::get_imp(axis)->range(std::forward<decltype(map)>(map), low, high);
    }

    void axis_copy_from(WrapperForAxis auto& axis, WrapperForAxis auto& other)
    {
        internal::get_imp(axis)->copyFrom(*internal::get_imp(other));
    }

    export class axis
    {
    public:
        explicit axis(axis_imp& axis)
            : imp { axis }
        {}

        [[nodiscard]] double draw_min() const { return axis_draw_min(*this); }
        [[nodiscard]] double draw_max() const { return axis_draw_max(*this); }
        [[nodiscard]] double draw_size() const { return axis_draw_size(*this); }

        [[maybe_unused]] axis& major(const tick& tick)
        {
            axis_set_major(*this, tick);
            return *this;
        }

        [[maybe_unused]] axis& majors(std::convertible_to<tick> auto&& ...ticks)
        {
            (major(std::forward<decltype(ticks)>(ticks)), ...);
            return *this;
        }

        [[maybe_unused]] axis& minor(const tick& tick)
        {
            axis_set_minor(*this, tick);
            return *this;
        }

        [[maybe_unused]] axis& minors(std::convertible_to<tick> auto&& ...ticks)
        {
            (minor(std::forward<decltype(ticks)>(ticks)), ...);
            return *this;
        }

        [[maybe_unused]] axis& tick(const tick& tick)
        {
            axis_set_tick(*this, tick);
            return *this;
        }

        [[maybe_unused]] axis& ticks(std::convertible_to<tick> auto&& ...ticks)
        {
            (tick(std::forward<decltype(ticks)>(ticks)), ...);
            return *this;
        }

        [[maybe_unused]] axis& linear(const double low, const double high)
        {
            axis_set_linear(*this, low, high);
            return *this;
        }

        [[maybe_unused]] axis& range(IsInvocableR<double, double> auto&& map)
        {
            axis_set_range(*this, std::forward<decltype(map)>(map));
            return *this;
        }

        [[maybe_unused]] axis& range(IsInvocableR<double, double> auto&& map, const double low, const double high)
        {
            axis_set_range(*this, std::forward<decltype(map)>(map), low, high);
            return *this;
        }

        [[maybe_unused]] axis& copy_from(axis& other)
        {
            axis_copy_from(*this, other);
            return *this;
        }

        [[maybe_unused]] axis& copy_from(axis&& other)
        {
            axis_copy_from(*this, other);
            return *this;
        }

    private:
        axis_imp& imp;

        friend struct internal;
        auto* get_imp() { return &imp; }
        auto const* get_imp() const { return &imp; }
    };

    export class const_axis
    {
    public:
        explicit const_axis(const axis_imp& axis)
            : imp { axis }
        {}

        [[nodiscard]] double draw_min() const { return axis_draw_min(*this); }
        [[nodiscard]] double draw_max() const { return axis_draw_max(*this); }
        [[nodiscard]] double draw_size() const { return axis_draw_size(*this); }

    private:
        const axis_imp& imp;

        friend struct internal;
        auto const* get_imp() { return &imp; }
        auto const* get_imp() const { return &imp; }
    };

}